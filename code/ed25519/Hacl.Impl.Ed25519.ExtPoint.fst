//
//   Copyright 2016-2017  INRIA
//
//   Maintainers: Jean-Karim Zinzindohoué
//                Karthikeyan Bhargavan
//                Benjamin Beurdouche
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//

module Hacl.Impl.Ed25519.ExtPoint

open FStar.Buffer


open Hacl.Bignum25519

#reset-options "--max_fuel 0 --z3rlimit 20"

(* A point is buffer of size 20, that is 4 consecutive buffers of size 5 *)
let point = b:Buffer.buffer Hacl.Bignum.Parameters.limb{Buffer.length b = 20}

let getx (p:point) : Tot Hacl.Bignum.Parameters.felem = Buffer.sub p 0ul 5ul
let gety (p:point) : Tot Hacl.Bignum.Parameters.felem = Buffer.sub p 5ul 5ul
let getz (p:point) : Tot Hacl.Bignum.Parameters.felem = Buffer.sub p 10ul 5ul
let gett (p:point) : Tot Hacl.Bignum.Parameters.felem = Buffer.sub p 15ul 5ul

(** Extracts the specification representation of the point from a heap and a pointer *)
val as_point: h:HyperStack.mem -> p:point{Buffer.live h p} -> GTot Spec.Ed25519.ext_point
let as_point h p =
  let x = Buffer.as_seq h (getx p) in
  let y = Buffer.as_seq h (gety p) in
  let z = Buffer.as_seq h (getz p) in
  let t = Buffer.as_seq h (gett p) in
  Hacl.Bignum25519.seval x, Hacl.Bignum25519.seval y, Hacl.Bignum25519.seval z, Hacl.Bignum25519.seval t


let point_inv h (p:point) : GTot Type0 =
  live h p /\ (let x = getx p in let y = gety p in let z = getz p in let t = gett p in
  red_513 (as_seq h x) /\ red_513 (as_seq h y) /\ red_513 (as_seq h z) /\ red_513 (as_seq h t))
