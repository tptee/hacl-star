module Loops

/// This file illustrates the use the new generalized iterators in
/// Hacl* libraries, namely ``Lib.Sequence.repeat`` and its Low*
/// counterpart `Lib.Buffer.loop``.
///
/// - ``repeat`` is a fold-like combinator that iterates a function
/// over a natural interval ``[lo..hi-1]`` starting from an initial
/// accumulator value.
///
/// A particularity is that the type of the accumulator depends on the
/// iteration index. This allows, for instance, to use as an
/// accumulator a sequence that grows iteratively, which is a common
/// pattern in Hacl*.
///
/// ``repeat`` is currently defined as ``repeat_right`` below, which
/// has a recursive definition that unfolds to an application of ``f``
/// at the head.
///
/// ```
/// val repeat_right:
///     lo:size_nat
///  -> hi:size_nat{lo <= hi}
///  -> a:(i:size_nat{lo <= i /\ i <= hi} -> Type)
///  -> f:(i:size_nat{lo <= i /\ i < hi} -> a i -> a (i + 1))
///  -> acc:a lo
///  -> a hi
/// let rec repeat_right lo hi a f acc =
///  if lo = hi then acc
///  else f (hi - 1) (repeat_right lo (hi - 1) a f acc)
/// ```
/// 
/// - ``loop`` is a wrapper around ``Lib.Loops.for`` that abstracts
/// over the Low* state used by the body. It takes as input a pure
/// function operating over an arbitrary index-dependent state (as in
/// ``repeat``), as well as a Low* implementation of this function.
///
/// The correspondence between the accumulator in the specification
/// and the Low* state is given by the ``refl`` parameter.
///
/// The memory footprint is specified as a ``loc`` (e.g. a set of
/// locations) also depending on the iteration index, so that the
/// footprint of a loop can grow iteratively -- consider for example
/// a loop that modifies an increasingly longer sub-buffer of an input
/// buffer.
/// 
///
/// ```
/// val loop:
///    h0:mem
/// -> n:size_t
/// -> a_spec:(i:size_nat{i <= v n} -> Type)
/// -> refl:(mem -> i:size_nat{i <= v n} -> GTot (a_spec i))
/// -> footprint:(i:size_nat{i <= v n} -> GTot loc)
/// -> spec:(mem -> GTot (i:size_nat{i < v n} -> a_spec i -> a_spec (i + 1)))
/// -> impl:(i:size_t{v i < v n} -> Stack unit
///    (requires loop_inv h0 n a_spec refl footprint spec (v i))
///    (ensures  fun _ _ -> loop_inv h0 n a_spec refl footprint spec (v i + 1)))
/// -> Stack unit
///   (requires fun h -> h0 == h)
///   (ensures  fun _ _ -> loop_inv h0 n a_spec refl footprint spec (v n))
/// ```
///
/// One quirk of ``loop`` is that it takes the heap upon entering the
/// loop ``h0`` as argument. It is tempting to avoid this by
/// defining
///
/// ```
/// let loop' n a_spec  refl footprint spec impl =
///   let h0 = ST.get() in
///   loop h0 n a_spec refl footprint spec (impl h0)
/// ```
///
/// However, this does not quite work for ``impl`` functions that
/// depend on the initial heap, and typically have e.g. liveness
/// pre-conditions. By essentially inlining this definition when using
/// ``loop``, we avoid having to state these pre-conditions
/// explicitly, which saves a lot of writing.
///

open FStar.HyperStack
open FStar.HyperStack.ST
open FStar.Mul

open Lib.IntTypes
open Lib.Sequence
open Lib.LoopCombinators
open Lib.Buffer

module ST = FStar.HyperStack.ST
module B = LowStar.Buffer
module Seq = Lib.Sequence

unfold let op_String_Access #a #len = Seq.op_String_Access #a #len
noextract unfold let repeat = repeat_gen
unfold let unfold_repeat = unfold_repeat_gen

#reset-options "--max_fuel 0 --max_ifuel 0"

/// Reverse
/// =======
///
/// We start with a loop that reverses a buffer by using an intermediate
/// buffer and copying the contents over to the input buffer.

/// We first specify the type of accumulator used in the
/// specification. Having an explicit definition is not
/// necessary. However, it is common to run into unification issues
/// when not omitting type annotations when using e.g. ``(fun n i ->
/// lseq uint8 n)``.
val reverse_state: n:size_nat -> i:size_nat{i <= n} -> Type0
let reverse_state n i = lseq uint8 n

/// Again, the foolproof method is to give a top-level definition
/// shared between the specification and implementation code.
noextract
val reverse_inner: n:size_nat -> s:lseq uint8 n -> i:size_nat{i < n} 
  -> reverse_state n i -> reverse_state n (i + 1)
let reverse_inner n s i t = t.[i] <- s.[n - i - 1]

/// This is the specification, using arbitrarily an all-zero initial sequence
noextract
val reverse_spec: n:size_nat -> s:lseq uint8 n -> lseq uint8 n
let reverse_spec n s =
  repeat n (reverse_state n) (reverse_inner n s) (Seq.create n (u8 0))

/// This is the Low* implementation, we choose to inline the ``refl``
/// and ``footprint`` parameters, but local ``let`` definitions would
/// work equally. Just remember to annotate with ``[@ inline_let]``
/// local definitions that aren't meant to be extracted. This is
/// always the case with the ``spec`` parameter. Forgetting this
/// attribute will result in Kremlin dropping the whole definition as
/// non-Low*. A telltale sign is Kremlin trying to extract
/// definitions in ``Lib.Sequence``.
val reverse: n:size_nat{0 < n} -> a:lbuffer uint8 n -> Stack unit
  (requires fun h0 -> B.live h0 a)
  (ensures  fun h0 _ h1 -> 
    B.modifies (B.loc_buffer a) h0 h1 /\ as_seq h1 a == reverse_spec n (as_seq h0 a))
let reverse n a =
  push_frame();
  // We use a local buffer as the Low* state
  let b = create (size n) (u8 0) in
  let h0 = ST.get() in
  // We annotate this so that Kremlin doesn't try to extract it
  [@ inline_let]
  let spec h0 = reverse_inner n (as_seq h0 a) in
  loop h0 (size n) (reverse_state n)
    /// We recover the spec accumulator by viewing the buffer as a sequence
    (fun h i -> as_seq h b)
    (fun i -> B.loc_buffer b)
    spec
    (fun i ->
      // This is the inductive hypothesis, for the sake of the example.
      // The explicit assertion is of course unnecessary
      let h1 = ST.get() in
      assert (as_seq h1 b ==
              repeat (v i) (reverse_state n) (spec h0) (Seq.create n (u8 0)));
      b.(i) <- a.(size n -! i -! size 1);
      let h2 = ST.get() in
      // This is what we know at this point from the IH
      assert (as_seq h2 b == spec h0 (v i)
        (repeat (v i) (reverse_state n) (spec h0) (Seq.create n (u8 0))));
      // And this is what we need to prove. This follows from the recursive
      // equation of ``repeat`, using 1 unit of fuel, but its definition is hidden
      // behind ``Lib.Sequence.fsti``.
      //
      // assert (as_seq h2 b ==        
      //  (repeat (v i + 1) (reverse_state n) (spec h0) (Seq.create n (u8 0))))
      //
      // We thus apply a lemma to unfold ``repeat``
      unfold_repeat_gen n (reverse_state n) (spec h0) (Seq.create n (u8 0)) (v i)
    );
  copy a (size n) b;
  pop_frame()


///  Reverse in place

val reverse_inplace_state: n:size_nat -> i:size_nat{i <= n / 2} -> Type0
let reverse_inplace_state n i = lseq uint8 n

noextract
val reverse_inplace_inner: n:size_nat -> i:size_nat{i < n / 2} 
  -> reverse_state n i -> reverse_state n (i + 1)
let reverse_inplace_inner n i s = 
  let tmp = s.[i] in
  let s = s.[i] <- s.[n - i - 1] in
  s.[n - i - 1] <- tmp

noextract
val reverse_inplace_spec: n:size_nat -> s:lseq uint8 n -> lseq uint8 n
let reverse_inplace_spec n s =
  repeat (n / 2) (reverse_inplace_state n) (reverse_inplace_inner n) s

val reverse_inplace: n:size_nat{0 < n} -> a:lbuffer uint8 n -> Stack unit
  (requires fun h0 -> B.live h0 a)
  (ensures  fun h0 _ h1 -> 
    B.modifies (B.loc_buffer a) h0 h1 /\ as_seq h1 a == 
    reverse_inplace_spec n (as_seq h0 a))
let reverse_inplace n a =
  push_frame();
  let h0 = ST.get() in
  loop h0 (size n /. size 2) (reverse_inplace_state n)
    (fun h i -> as_seq h a)
    (fun i -> B.loc_buffer a)
    (fun h0 -> reverse_inplace_inner n)
    (fun i ->
      unfold_repeat (n / 2) (reverse_inplace_state n) (reverse_inplace_inner n) (as_seq h0 a) (v i);
      let tmp = a.(i) in
      a.(i) <- a.(size n -! i -! size 1);
      a.(size n -! i -! size 1) <- tmp
    );
  pop_frame()

///  Tail-recursive Fibonnacci

val fib_state: n:nat -> i:nat{i <= n} -> Type0
let fib_state n i = nat & nat

noextract
val fib_inner: n:nat -> i:nat{i < n} -> fib_state n i -> fib_state n (i + 1)
let fib_inner n i (a, b) = b, a + b

noextract
val fib_spec: nat -> nat
let fib_spec n = fst (repeat n (fib_state n) (fib_inner n) (0, 1))

// We use ``nat`` rather than a machine integer type to keep things simple
val fib: n:size_nat -> Stack nat
  (requires fun _ -> True)
  (ensures  fun h0 res h1 -> modifies B.loc_none h0 h1 /\ res == fib_spec n)
let fib n =
  push_frame();
  let a:lbuffer nat 1 = create (size 1) 0 in
  let b:lbuffer nat 1 = create (size 1) 1 in
  let h0 = ST.get () in
  loop h0 (size n) (fib_state n)
  (fun h _ -> bget h a 0, bget h b 0)
  (fun _ -> B.loc_union (B.loc_buffer a) (B.loc_buffer b))
  (fun _ -> fib_inner n)
  (fun i ->
    unfold_repeat n (fib_state n) (fib_inner n) (0,1) (size_v i);
    let a_ = a.(size 0) in
    a.(size 0) <- b.(size 0); 
    b.(size 0) <- a_ + b.(size 0) 
  );
  let res = a.(size 0) in
  pop_frame();
  res

/// Tests

let b0: b:lbuffer uint8 3{B.recallable b} =
  [@inline_let]
  let l = [u8 1; u8 2; u8 3] in
  assert_norm (List.Tot.length l <= max_size_t);
  createL_global l

let b1: b:lbuffer uint8 3{B.recallable b} = 
  [@inline_let]
  let l = [u8 1; u8 2; u8 3] in
  assert_norm (List.Tot.length l <= max_size_t);
  createL_global l
  
let b2: b:lbuffer uint8 3{B.recallable b} = 
  [@inline_let]
  let l = [u8 3; u8 2; u8 1] in
  assert_norm (List.Tot.length l <= max_size_t);
  createL_global l

let a0: b:lbuffer uint8 4{B.recallable b} = 
  [@inline_let]
  let l = [u8 1; u8 2; u8 3; u8 4] in
  assert_norm (List.Tot.length l <= max_size_t);
  createL_global l

let a1: b:lbuffer uint8 4{B.recallable b} = 
  [@inline_let]
  let l = [u8 1; u8 2; u8 3; u8 4] in
  assert_norm (List.Tot.length l <= max_size_t);
  createL_global l

let a2: b:lbuffer uint8 4{B.recallable b} = 
  [@inline_let]
  let l = [u8 4; u8 3; u8 2; u8 1] in
  assert_norm (List.Tot.length l <= max_size_t);
  createL_global l

val main: unit -> St int
let main () =
  recall b0;
  reverse_inplace 3 b0;  
  print_compare_display (size 3) b0 b2;
  recall b0;
  reverse 3 b0;
  print_compare_display (size 3) b0 b1;
  recall a0;
  reverse_inplace 4 a0; 
  print_compare_display (size 4) a0 a2;
  recall a0;
  reverse 4 a0;
  print_compare_display (size 4) a0 a1;
  let open TestLib in
  let n = fib 10 in 
  assume (UInt.fits (fib_spec 10) 32);
  TestLib.checku32 (UInt32.uint_to_t n) 55ul;
  0
