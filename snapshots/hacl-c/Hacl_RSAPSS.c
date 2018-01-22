/* This file was auto-generated by KreMLin! */

#include "Hacl_RSAPSS.h"
#include "Hacl_SHA2_256.h"

static bool
Hacl_Impl_Lib_bn_is_bit_set(Prims_nat len, uint32_t clen, uint64_t *input, uint32_t ind)
{
  uint32_t i = ind / (uint32_t)64U;
  uint32_t j = ind % (uint32_t)64U;
  uint64_t tmp = input[i];
  uint64_t x0 = tmp >> j & (uint64_t)1U;
  return x0 == (uint64_t)1U;
}

static void
Hacl_Impl_Lib_bn_set_bit(Prims_nat len, uint32_t clen, uint64_t *input, uint32_t ind)
{
  uint32_t i = ind / (uint32_t)64U;
  uint32_t j = ind % (uint32_t)64U;
  uint64_t tmp = input[i];
  input[i] = tmp | (uint64_t)1U << j;
}

static void Hacl_Impl_Lib_fill(Prims_nat len, uint32_t clen, uint64_t *b, uint64_t z)
{
  for (uint32_t i = (uint32_t)0U; i < clen; i = i + (uint32_t)1U)
    b[i] = z;
}

static FStar_UInt128_t Hacl_Impl_Lib_mul_wide(uint64_t a, uint64_t b)
{
  return FStar_UInt128_mul_wide(a, b);
}

static bool
Hacl_Impl_Lib_eq_b_(Prims_nat len, uint32_t clen, uint8_t *b1, uint8_t *b2, uint32_t i)
{
  if (i < clen)
  {
    uint8_t uu____851 = b1[i];
    uint8_t uu____888 = b2[i];
    bool uu____850 = uu____851 == uu____888;
    if (uu____850)
      return Hacl_Impl_Lib_eq_b_(len, clen, b1, b2, i + (uint32_t)1U);
    else
      return false;
  }
  else
    return true;
}

bool Hacl_Impl_Lib_eq_b(Prims_nat len, uint32_t clen, uint8_t *b1, uint8_t *b2)
{
  return Hacl_Impl_Lib_eq_b_(len, clen, b1, b2, (uint32_t)0U);
}

static void
Hacl_Impl_Addition_bn_sub_(
  Prims_nat aLen,
  Prims_nat bLen,
  uint32_t caLen,
  uint64_t *a,
  uint32_t cbLen,
  uint64_t *b,
  uint32_t i,
  uint64_t carry,
  uint64_t *res
)
{
  if (i < caLen)
  {
    uint64_t t1 = a[i];
    uint64_t t2;
    if (i < cbLen)
      t2 = b[i];
    else
      t2 = (uint64_t)0U;
    uint64_t res_i = t1 - t2 - carry;
    res[i] = res_i;
    uint64_t carry1;
    if (carry == (uint64_t)1U)
    {
      uint64_t ite;
      if (t1 <= t2)
        ite = (uint64_t)1U;
      else
        ite = (uint64_t)0U;
      carry1 = ite;
    }
    else
    {
      uint64_t ite;
      if (t1 < t2)
        ite = (uint64_t)1U;
      else
        ite = (uint64_t)0U;
      carry1 = ite;
    }
    Hacl_Impl_Addition_bn_sub_(aLen, bLen, caLen, a, cbLen, b, i + (uint32_t)1U, carry1, res);
  }
}

static void
Hacl_Impl_Addition_bn_sub(
  Prims_nat aLen,
  Prims_nat bLen,
  uint32_t caLen,
  uint64_t *a,
  uint32_t cbLen,
  uint64_t *b,
  uint64_t *res
)
{
  Hacl_Impl_Addition_bn_sub_(aLen, bLen, caLen, a, cbLen, b, (uint32_t)0U, (uint64_t)0U, res);
}

static uint64_t
Hacl_Impl_Addition_bn_add_(
  Prims_nat aLen,
  Prims_nat bLen,
  uint32_t caLen,
  uint64_t *a,
  uint32_t cbLen,
  uint64_t *b,
  uint32_t i,
  uint64_t carry,
  uint64_t *res
)
{
  if (i < caLen)
  {
    uint64_t t1 = a[i];
    uint64_t t2;
    if (i < cbLen)
      t2 = b[i];
    else
      t2 = (uint64_t)0U;
    uint64_t res_i = t1 + t2 + carry;
    res[i] = res_i;
    uint64_t carry1;
    if (res_i < t1)
      carry1 = (uint64_t)1U;
    else
      carry1 = (uint64_t)0U;
    return
      Hacl_Impl_Addition_bn_add_(aLen,
        bLen,
        caLen,
        a,
        cbLen,
        b,
        i + (uint32_t)1U,
        carry1,
        res);
  }
  else
    return carry;
}

static void
Hacl_Impl_Addition_bn_add(
  Prims_nat aLen,
  Prims_nat bLen,
  uint32_t caLen,
  uint64_t *a,
  uint32_t cbLen,
  uint64_t *b,
  uint64_t *res
)
{
  (void)Hacl_Impl_Addition_bn_add_(aLen,
    bLen,
    caLen,
    a,
    cbLen,
    b,
    (uint32_t)0U,
    (uint64_t)0U,
    res);
}

static void
Hacl_Impl_Addition_bn_add_carry(
  Prims_nat aLen,
  Prims_nat bLen,
  uint32_t caLen,
  uint64_t *a,
  uint32_t cbLen,
  uint64_t *b,
  uint64_t *res
)
{
  uint64_t *res_ = res;
  uint64_t
  carry =
    Hacl_Impl_Addition_bn_add_(aLen,
      bLen,
      caLen,
      a,
      cbLen,
      b,
      (uint32_t)0U,
      (uint64_t)0U,
      res_);
  res[caLen] = carry;
}

static bool
Hacl_Impl_Comparison_bn_is_less_(
  Prims_nat len,
  uint32_t clen,
  uint64_t *a,
  uint64_t *b,
  uint32_t i
)
{
  if (i > (uint32_t)0U)
  {
    uint32_t i1 = i - (uint32_t)1U;
    uint64_t t1 = a[i1];
    uint64_t t2 = b[i1];
    if (!(t1 == t2))
      if (t1 < t2)
        return true;
      else
        return false;
    else
      return Hacl_Impl_Comparison_bn_is_less_(len, clen, a, b, i1);
  }
  else
    return false;
}

static bool
Hacl_Impl_Comparison_bn_is_less(Prims_nat len, uint32_t clen, uint64_t *a, uint64_t *b)
{
  return Hacl_Impl_Comparison_bn_is_less_(len, clen, a, b, clen);
}

static void
Hacl_Impl_Convert_text_to_nat_(
  Prims_nat len,
  uint32_t clen,
  uint8_t *input,
  uint32_t resLen,
  uint64_t *res,
  uint32_t i
)
{
  if (i < resLen)
  {
    uint64_t inputi = load64_be(input + (uint32_t)8U * i);
    uint32_t ind = resLen - i - (uint32_t)1U;
    res[ind] = inputi;
    Hacl_Impl_Convert_text_to_nat_(len, clen, input, resLen, res, i + (uint32_t)1U);
  }
}

void Hacl_Impl_Convert_text_to_nat(Prims_nat len, uint32_t clen, uint8_t *input, uint64_t *res)
{
  uint32_t num_words = (clen - (uint32_t)1U) / (uint32_t)8U + (uint32_t)1U;
  uint32_t tmpLen = (uint32_t)8U * num_words;
  uint32_t m = clen % (uint32_t)8U;
  uint32_t ind;
  if (m == (uint32_t)0U)
    ind = (uint32_t)0U;
  else
    ind = (uint32_t)8U - m;
  KRML_CHECK_SIZE((uint8_t)0U, tmpLen);
  uint8_t buf[tmpLen];
  memset(buf, 0U, tmpLen * sizeof buf[0U]);
  (void)(tmpLen - ind);
  uint8_t *tmp_ = buf + ind;
  for (uint32_t i = (uint32_t)0U; i < clen; i = i + (uint32_t)1U)
  {
    uint8_t src_i = input[i];
    tmp_[i] = src_i;
  }
  Hacl_Impl_Convert_text_to_nat_(FStar_UInt32_v(tmpLen),
    tmpLen,
    buf,
    num_words,
    res,
    (uint32_t)0U);
}

static void
Hacl_Impl_Convert_nat_to_text_(
  Prims_nat len,
  uint32_t clen,
  uint64_t *input,
  uint32_t resLen,
  uint8_t *res,
  uint32_t i
)
{
  if (i < clen)
  {
    uint32_t ind = clen - i - (uint32_t)1U;
    uint64_t tmp = input[ind];
    store64_be(res + (uint32_t)8U * i, tmp);
    Hacl_Impl_Convert_nat_to_text_(len, clen, input, resLen, res, i + (uint32_t)1U);
  }
}

static void
Hacl_Impl_Convert_nat_to_text(Prims_nat len, uint32_t clen, uint64_t *input, uint8_t *res)
{
  uint32_t num_words = (clen - (uint32_t)1U) / (uint32_t)8U + (uint32_t)1U;
  uint32_t tmpLen = (uint32_t)8U * num_words;
  uint32_t m = clen % (uint32_t)8U;
  uint32_t ind;
  if (m == (uint32_t)0U)
    ind = (uint32_t)0U;
  else
    ind = (uint32_t)8U - m;
  KRML_CHECK_SIZE((uint8_t)0U, tmpLen);
  uint8_t buf[tmpLen];
  memset(buf, 0U, tmpLen * sizeof buf[0U]);
  Hacl_Impl_Convert_nat_to_text_(FStar_UInt32_v((clen - (uint32_t)1U)
      / (uint32_t)8U
      + (uint32_t)1U),
    num_words,
    input,
    tmpLen,
    buf,
    (uint32_t)0U);
  (void)(tmpLen - ind);
  uint8_t *tmp_ = buf + ind;
  for (uint32_t i = (uint32_t)0U; i < clen; i = i + (uint32_t)1U)
  {
    uint8_t src_i = tmp_[i];
    res[i] = src_i;
  }
}

static void
Hacl_Impl_Multiplication_bn_mult_by_limb_addj(
  Prims_nat aLen,
  uint32_t aaLen,
  uint64_t *a,
  uint64_t l,
  uint32_t i,
  uint32_t j,
  uint32_t resLen,
  uint64_t carry,
  uint64_t *res
)
{
  uint32_t ij = i + j;
  if (i < aaLen)
  {
    uint64_t res_ij = res[ij];
    uint64_t uu____258 = a[i];
    K___uint64_t_uint64_t
    uu____253 =
      {
        .fst = FStar_UInt128_uint128_to_uint64(FStar_UInt128_shift_right(FStar_UInt128_add(FStar_UInt128_add(Hacl_Impl_Lib_mul_wide(uu____258,
                  l),
                FStar_UInt128_uint64_to_uint128(carry)),
              FStar_UInt128_uint64_to_uint128(res_ij)),
            (uint32_t)64U)),
        .snd = FStar_UInt128_uint128_to_uint64(FStar_UInt128_add(FStar_UInt128_add(Hacl_Impl_Lib_mul_wide(uu____258,
                l),
              FStar_UInt128_uint64_to_uint128(carry)),
            FStar_UInt128_uint64_to_uint128(res_ij)))
      };
    K___uint64_t_uint64_t scrut = uu____253;
    uint64_t carry_ = scrut.fst;
    uint64_t res_ij1 = scrut.snd;
    res[ij] = res_ij1;
    Hacl_Impl_Multiplication_bn_mult_by_limb_addj(aLen,
      aaLen,
      a,
      l,
      i + (uint32_t)1U,
      j,
      resLen,
      carry_,
      res);
  }
  else
    res[ij] = carry;
}

static void
Hacl_Impl_Multiplication_bn_mult_(
  Prims_nat aLen,
  Prims_nat bLen,
  uint32_t aaLen,
  uint64_t *a,
  uint32_t bbLen,
  uint64_t *b,
  uint32_t j,
  uint32_t resLen,
  uint64_t *res
)
{
  if (j < bbLen)
  {
    uint64_t uu____705 = b[j];
    Hacl_Impl_Multiplication_bn_mult_by_limb_addj(aLen,
      aaLen,
      a,
      uu____705,
      (uint32_t)0U,
      j,
      resLen,
      (uint64_t)0U,
      res);
    Hacl_Impl_Multiplication_bn_mult_(aLen,
      bLen,
      aaLen,
      a,
      bbLen,
      b,
      j + (uint32_t)1U,
      resLen,
      res);
  }
}

static void
Hacl_Impl_Multiplication_bn_mul(
  Prims_nat aLen,
  Prims_nat bLen,
  uint32_t aaLen,
  uint64_t *a,
  uint32_t bbLen,
  uint64_t *b,
  uint64_t *res
)
{
  Hacl_Impl_Multiplication_bn_mult_(aLen,
    bLen,
    aaLen,
    a,
    bbLen,
    b,
    (uint32_t)0U,
    aaLen + bbLen,
    res);
}

static void
Hacl_Impl_Multiplication_abs(
  Prims_nat aLen,
  uint32_t aaLen,
  uint64_t *a,
  uint64_t *b,
  uint64_t *res
)
{
  bool uu____1505 = Hacl_Impl_Comparison_bn_is_less(aLen, aaLen, a, b);
  if (uu____1505)
    Hacl_Impl_Addition_bn_sub(aLen, aLen, aaLen, b, aaLen, a, res);
  else
    Hacl_Impl_Addition_bn_sub(aLen, aLen, aaLen, a, aaLen, b, res);
}

static void
Hacl_Impl_Multiplication_add_sign(
  Prims_nat aLen,
  Prims_nat cLen,
  Prims_nat resLen,
  uint32_t aaLen,
  uint32_t ccLen,
  uint32_t rresLen,
  uint64_t *c0,
  uint64_t *c1,
  uint64_t *c2,
  uint64_t *a0,
  uint64_t *a1,
  uint64_t *a2,
  uint64_t *b0,
  uint64_t *b1,
  uint64_t *b2,
  uint64_t *res
)
{
  bool uu____2517 = Hacl_Impl_Comparison_bn_is_less(aLen, aaLen, a0, a1);
  Hacl_Impl_Multiplication_sign sa2;
  if (uu____2517)
    sa2 = Hacl_Impl_Multiplication_Negative;
  else
    sa2 = Hacl_Impl_Multiplication_Positive;
  bool uu____2598 = Hacl_Impl_Comparison_bn_is_less(aLen, aaLen, b0, b1);
  Hacl_Impl_Multiplication_sign sb2;
  if (uu____2598)
    sb2 = Hacl_Impl_Multiplication_Negative;
  else
    sb2 = Hacl_Impl_Multiplication_Positive;
  Hacl_Impl_Addition_bn_add_carry(cLen, cLen, ccLen, c0, ccLen, c1, res);
  if
  (sa2
  == Hacl_Impl_Multiplication_Positive
  && sb2 == Hacl_Impl_Multiplication_Positive
  || sa2 == Hacl_Impl_Multiplication_Negative && sb2 == Hacl_Impl_Multiplication_Negative)
    Hacl_Impl_Addition_bn_sub(FStar_UInt32_v(rresLen), cLen, rresLen, res, ccLen, c2, res);
  else
    Hacl_Impl_Addition_bn_add(FStar_UInt32_v(rresLen), cLen, rresLen, res, ccLen, c2, res);
}

static void
Hacl_Impl_Multiplication_karatsuba_(
  Prims_nat aLen,
  uint32_t aaLen,
  uint64_t *a,
  uint64_t *b,
  uint64_t *tmp,
  uint64_t *res
)
{
  (void)((uint32_t)4U * aaLen);
  uint32_t aaLen2 = aaLen + aaLen;
  uint32_t aaLen0 = aaLen / (uint32_t)2U;
  Hacl_Impl_Lib_fill(Prims_op_Multiply((krml_checked_int_t)2, aLen), aaLen2, res, (uint64_t)0U);
  if (aaLen < (uint32_t)100U)
    Hacl_Impl_Multiplication_bn_mul(aLen, aLen, aaLen, a, aaLen, b, res);
  else
  {
    uint64_t *a0 = a;
    uint64_t *a1 = a + aaLen0;
    uint64_t *b0 = b;
    uint64_t *b1 = b + aaLen0;
    uint64_t *tmp0 = tmp;
    uint64_t *c0 = res;
    uint64_t *c1 = res + aaLen;
    Hacl_Impl_Multiplication_karatsuba_(FStar_UInt32_v(aaLen0), aaLen0, a0, b0, tmp0, c0);
    Hacl_Impl_Multiplication_karatsuba_(FStar_UInt32_v(aaLen0), aaLen0, a1, b1, tmp0, c1);
    uint64_t *a2 = tmp;
    uint64_t *b2 = tmp + aaLen0;
    uint64_t *c2 = tmp + aaLen;
    uint64_t *tmp01 = tmp + aaLen2;
    Hacl_Impl_Multiplication_abs(FStar_UInt32_v(aaLen0), aaLen0, a0, a1, a2);
    Hacl_Impl_Multiplication_abs(FStar_UInt32_v(aaLen0), aaLen0, b0, b1, b2);
    Hacl_Impl_Multiplication_karatsuba_(FStar_UInt32_v(aaLen0), aaLen0, a2, b2, tmp01, c2);
    uint32_t tmp1Len = aaLen + (uint32_t)1U;
    uint64_t *tmp1 = tmp + aaLen2;
    Hacl_Impl_Multiplication_add_sign(FStar_UInt32_v(aaLen0),
      FStar_UInt32_v(aaLen),
      FStar_UInt32_v(tmp1Len),
      aaLen0,
      aaLen,
      tmp1Len,
      c0,
      c1,
      c2,
      a0,
      a1,
      a2,
      b0,
      b1,
      b2,
      tmp1);
    uint32_t res1Len = aaLen0 + aaLen;
    uint64_t *res1 = res + aaLen0;
    Hacl_Impl_Addition_bn_add(FStar_UInt32_v(res1Len),
      FStar_UInt32_v(tmp1Len),
      res1Len,
      res1,
      tmp1Len,
      tmp1,
      res1);
  }
}

static void
Hacl_Impl_Multiplication_karatsuba(
  Prims_nat aLen,
  uint32_t iLen,
  uint32_t aaLen,
  uint64_t *a,
  uint64_t *b,
  uint64_t *st_mult
)
{
  (void)(aaLen + aaLen);
  (void)((uint32_t)4U * iLen);
  uint32_t stLen = (uint32_t)8U * iLen;
  Hacl_Impl_Lib_fill(Prims_op_Multiply((krml_checked_int_t)8, FStar_UInt32_v(iLen)),
    stLen,
    st_mult,
    (uint64_t)0U);
  if (aaLen < (uint32_t)9U)
  {
    uint64_t *res = st_mult;
    Hacl_Impl_Multiplication_bn_mul(aLen, aLen, aaLen, a, aaLen, b, res);
  }
  else
  {
    uint64_t *res_ = st_mult;
    uint64_t *a_ = st_mult + iLen + iLen;
    uint64_t *b_ = st_mult + (uint32_t)3U * iLen;
    uint64_t *tmp = st_mult + (uint32_t)4U * iLen;
    uint64_t *a__ = a_;
    uint64_t *b__ = b_;
    for (uint32_t i = (uint32_t)0U; i < aaLen; i = i + (uint32_t)1U)
    {
      uint64_t src_i = a[i];
      a__[i] = src_i;
    }
    for (uint32_t i = (uint32_t)0U; i < aaLen; i = i + (uint32_t)1U)
    {
      uint64_t src_i = b[i];
      b__[i] = src_i;
    }
    Hacl_Impl_Multiplication_karatsuba_(FStar_UInt32_v(iLen), iLen, a_, b_, tmp, res_);
  }
}

static uint64_t Hacl_Impl_Shift_bn_tbit = (uint64_t)0x8000000000000000U;

static void
Hacl_Impl_Shift_bn_lshift1_(
  Prims_nat aLen,
  uint32_t caLen,
  uint64_t *a,
  uint64_t carry,
  uint32_t i,
  uint64_t *res
)
{
  if (i < caLen)
  {
    uint64_t tmp = a[i];
    res[i] = tmp << (uint32_t)1U | carry;
    uint64_t carry1;
    uint64_t x0 = tmp & Hacl_Impl_Shift_bn_tbit;
    if (x0 == Hacl_Impl_Shift_bn_tbit)
      carry1 = (uint64_t)1U;
    else
      carry1 = (uint64_t)0U;
    Hacl_Impl_Shift_bn_lshift1_(aLen, caLen, a, carry1, i + (uint32_t)1U, res);
  }
}

static void
Hacl_Impl_Shift_bn_lshift1(Prims_nat aLen, uint32_t caLen, uint64_t *a, uint64_t *res)
{
  Hacl_Impl_Shift_bn_lshift1_(aLen, caLen, a, (uint64_t)0U, (uint32_t)0U, res);
}

static void
Hacl_Impl_Shift_bn_rshift_(
  Prims_nat aLen,
  uint32_t caLen,
  uint64_t *a,
  uint32_t i,
  uint32_t nw,
  uint32_t rb,
  uint64_t l,
  uint64_t *res
)
{
  if (i < caLen - nw)
  {
    uint64_t tmp = l >> rb;
    uint64_t l1 = a[nw + i];
    uint32_t lb = (uint32_t)64U - rb;
    res[i - (uint32_t)1U] = tmp | l1 << lb;
    Hacl_Impl_Shift_bn_rshift_(aLen, caLen, a, i + (uint32_t)1U, nw, rb, l1, res);
  }
  else
    res[i - (uint32_t)1U] = l >> rb;
}

static void
Hacl_Impl_Shift_bn_rshift(
  Prims_nat aLen,
  uint32_t caLen,
  uint64_t *a,
  uint32_t nCount,
  uint64_t *res
)
{
  uint32_t nw = nCount / (uint32_t)64U;
  uint32_t rb = nCount % (uint32_t)64U;
  if (rb == (uint32_t)0U)
  {
    uint32_t a_Len = caLen - nw;
    uint64_t *a_ = a + nw;
    uint64_t *res_ = res;
    for (uint32_t i = (uint32_t)0U; i < a_Len; i = i + (uint32_t)1U)
    {
      uint64_t src_i = a_[i];
      res_[i] = src_i;
    }
  }
  else
  {
    uint64_t l = a[nw];
    Hacl_Impl_Shift_bn_rshift_(aLen, caLen, a, (uint32_t)1U, nw, rb, l, res);
  }
}

static void
Hacl_Impl_Shift_bn_mod_pow2_n(
  Prims_nat aLen,
  Prims_nat resLen,
  uint32_t caLen,
  uint64_t *a,
  uint32_t nCount,
  uint32_t cresLen,
  uint64_t *res
)
{
  uint32_t nw = nCount / (uint32_t)64U;
  uint32_t nb = nCount % (uint32_t)64U;
  uint64_t *a_ = a;
  for (uint32_t i = (uint32_t)0U; i < cresLen; i = i + (uint32_t)1U)
  {
    uint64_t src_i = a_[i];
    res[i] = src_i;
  }
  uint32_t start_i;
  if (nb > (uint32_t)0U)
  {
    uint32_t lb = (uint32_t)64U - nb;
    uint64_t uu____1732 = res[nw];
    uint64_t uu____1731 = uu____1732 & (uint64_t)0xffffffffffffffffU >> lb;
    res[nw] = uu____1731;
    start_i = nw + (uint32_t)1U;
  }
  else
    start_i = nw;
  if (start_i < cresLen)
  {
    uint32_t res_Len = cresLen - start_i;
    uint64_t *res_ = res + start_i;
    Hacl_Impl_Lib_fill(FStar_UInt32_v(res_Len), res_Len, res_, (uint64_t)0U);
  }
}

static void
Hacl_Impl_Montgomery_bn_pow2_mod_n_(
  Prims_nat rLen,
  uint32_t clen,
  uint64_t *a,
  uint32_t i,
  uint32_t p,
  uint64_t *r
)
{
  if (i < p)
  {
    Hacl_Impl_Shift_bn_lshift1(rLen, clen, r, r);
    bool uu____251 = Hacl_Impl_Comparison_bn_is_less(rLen, clen, r, a);
    bool uu____250 = !uu____251;
    if (uu____250)
      Hacl_Impl_Addition_bn_sub(rLen, rLen, clen, r, clen, a, r);
    Hacl_Impl_Montgomery_bn_pow2_mod_n_(rLen, clen, a, i + (uint32_t)1U, p, r);
  }
}

static void
Hacl_Impl_Montgomery_bn_pow2_mod_n(
  Prims_nat rLen,
  uint32_t aBits,
  uint32_t rLen1,
  uint64_t *a,
  uint32_t p,
  uint64_t *r
)
{
  Hacl_Impl_Lib_bn_set_bit(rLen, rLen1, r, aBits);
  Hacl_Impl_Addition_bn_sub(rLen, rLen, rLen1, r, rLen1, a, r);
  Hacl_Impl_Montgomery_bn_pow2_mod_n_(rLen, rLen1, a, aBits, p, r);
}

static void
Hacl_Impl_Montgomery_mont_inverse_(
  Prims_nat rLen,
  Prims_nat stLen,
  uint32_t crLen,
  uint64_t *n1,
  uint32_t exp_2,
  uint32_t i,
  uint32_t cstLen,
  uint64_t *st
)
{
  uint32_t cr2Len = crLen + crLen;
  uint64_t *pow2_i1 = st;
  uint64_t *pow2_i = st + crLen;
  uint64_t *nnInv = st + cr2Len;
  uint64_t *nnInv_mod = st + (uint32_t)4U * crLen;
  uint64_t *nInv = st + (uint32_t)5U * crLen;
  if (i <= exp_2)
  {
    Hacl_Impl_Shift_bn_lshift1(rLen, crLen, pow2_i1, pow2_i1);
    Hacl_Impl_Shift_bn_lshift1(rLen, crLen, pow2_i1, pow2_i);
    Hacl_Impl_Lib_fill(FStar_UInt32_v(cr2Len), cr2Len, nnInv, (uint64_t)0U);
    Hacl_Impl_Multiplication_bn_mul(rLen, rLen, crLen, n1, crLen, nInv, nnInv);
    Hacl_Impl_Shift_bn_mod_pow2_n(FStar_UInt32_v(cr2Len),
      rLen,
      cr2Len,
      nnInv,
      i,
      crLen,
      nnInv_mod);
    bool uu____1532 = Hacl_Impl_Comparison_bn_is_less(rLen, crLen, pow2_i1, nnInv_mod);
    if (uu____1532)
      Hacl_Impl_Addition_bn_add(rLen, rLen, crLen, nInv, crLen, pow2_i1, nInv);
    Hacl_Impl_Montgomery_mont_inverse_(rLen,
      stLen,
      crLen,
      n1,
      exp_2,
      i + (uint32_t)1U,
      cstLen,
      st);
  }
}

static void
Hacl_Impl_Montgomery_mont_inverse(
  Prims_nat rLen,
  uint32_t crLen,
  uint64_t *n1,
  uint32_t exp_2,
  uint64_t *nInv
)
{
  uint32_t stLen = crLen * (uint32_t)6U;
  KRML_CHECK_SIZE((uint64_t)0U, stLen);
  uint64_t buf[stLen];
  memset(buf, 0U, stLen * sizeof buf[0U]);
  uint64_t *pow2_i1 = buf;
  uint64_t *nInv_t = buf + (uint32_t)5U * crLen;
  pow2_i1[0U] = (uint64_t)1U;
  nInv_t[0U] = (uint64_t)1U;
  Hacl_Impl_Montgomery_mont_inverse_(rLen,
    FStar_UInt32_v(stLen),
    crLen,
    n1,
    exp_2,
    (uint32_t)2U,
    stLen,
    buf);
  for (uint32_t i = (uint32_t)0U; i < crLen; i = i + (uint32_t)1U)
  {
    uint64_t src_i = nInv_t[i];
    nInv[i] = src_i;
  }
}

static void
Hacl_Impl_Montgomery_mont_reduction(
  Prims_nat rLen,
  Prims_nat cLen,
  uint32_t exp_r,
  uint32_t rrLen,
  uint64_t *st_mont,
  uint64_t *st,
  uint32_t ccLen,
  uint64_t *c,
  uint64_t *res
)
{
  uint32_t r2Len = rrLen + rrLen;
  uint32_t stLen = (uint32_t)3U * rrLen;
  Hacl_Impl_Lib_fill(Prims_op_Multiply((krml_checked_int_t)3, rLen), stLen, st, (uint64_t)0U);
  uint64_t *tmp1 = st;
  uint64_t *m = st + r2Len;
  uint64_t *r = st_mont;
  uint64_t *n1 = st_mont + rrLen;
  uint64_t *nInv = st_mont + r2Len;
  uint64_t *c1 = c;
  Hacl_Impl_Multiplication_bn_mul(rLen, rLen, rrLen, c1, rrLen, nInv, tmp1);
  Hacl_Impl_Shift_bn_mod_pow2_n(FStar_UInt32_v(r2Len), rLen, r2Len, tmp1, exp_r, rrLen, m);
  Hacl_Impl_Addition_bn_sub(rLen, rLen, rrLen, r, rrLen, m, m);
  Hacl_Impl_Lib_fill(FStar_UInt32_v(r2Len), r2Len, tmp1, (uint64_t)0U);
  Hacl_Impl_Multiplication_bn_mul(rLen, rLen, rrLen, m, rrLen, n1, tmp1);
  Hacl_Impl_Addition_bn_add(FStar_UInt32_v(r2Len), cLen, r2Len, tmp1, ccLen, c, tmp1);
  Hacl_Impl_Shift_bn_rshift(FStar_UInt32_v(r2Len), r2Len, tmp1, exp_r, tmp1);
  uint64_t *tmp1_ = tmp1;
  for (uint32_t i = (uint32_t)0U; i < rrLen; i = i + (uint32_t)1U)
  {
    uint64_t src_i = tmp1_[i];
    res[i] = src_i;
  }
}

static void
Hacl_Impl_Montgomery_to_mont(
  Prims_nat rLen,
  uint32_t exp_r,
  uint32_t rrLen,
  uint64_t *st_mont,
  uint64_t *st,
  uint64_t *r2,
  uint64_t *a,
  uint64_t *aM
)
{
  uint32_t cLen = rrLen + rrLen;
  (void)((uint32_t)3U * rrLen);
  uint64_t *c = st;
  uint64_t *st1 = st + cLen;
  Hacl_Impl_Lib_fill(FStar_UInt32_v(cLen), cLen, c, (uint64_t)0U);
  Hacl_Impl_Multiplication_bn_mul(rLen, rLen, rrLen, a, rrLen, r2, c);
  Hacl_Impl_Montgomery_mont_reduction(rLen,
    FStar_UInt32_v(cLen),
    exp_r,
    rrLen,
    st_mont,
    st1,
    cLen,
    c,
    aM);
}

static void
Hacl_Impl_Montgomery_from_mont(
  Prims_nat rLen,
  uint32_t exp_r,
  uint32_t rrLen,
  uint64_t *st_mont,
  uint64_t *st,
  uint64_t *aM,
  uint64_t *a
)
{
  uint32_t r2Len = rrLen + rrLen;
  uint32_t stLen = (uint32_t)3U * rrLen;
  Hacl_Impl_Lib_fill(Prims_op_Multiply((krml_checked_int_t)3, rLen), stLen, st, (uint64_t)0U);
  uint64_t *tmp = st;
  uint64_t *m = st + r2Len;
  uint64_t *r = st_mont;
  uint64_t *n1 = st_mont + rrLen;
  uint64_t *nInv = st_mont + r2Len;
  Hacl_Impl_Multiplication_bn_mul(rLen, rLen, rrLen, aM, rrLen, nInv, tmp);
  Hacl_Impl_Shift_bn_mod_pow2_n(FStar_UInt32_v(r2Len), rLen, r2Len, tmp, exp_r, rrLen, m);
  Hacl_Impl_Addition_bn_sub(rLen, rLen, rrLen, r, rrLen, m, m);
  Hacl_Impl_Lib_fill(FStar_UInt32_v(r2Len), r2Len, tmp, (uint64_t)0U);
  Hacl_Impl_Multiplication_bn_mul(rLen, rLen, rrLen, m, rrLen, n1, tmp);
  Hacl_Impl_Addition_bn_add(FStar_UInt32_v(r2Len), rLen, r2Len, tmp, rrLen, aM, tmp);
  Hacl_Impl_Shift_bn_rshift(FStar_UInt32_v(r2Len), r2Len, tmp, exp_r, tmp);
  uint64_t *tmp_ = tmp;
  for (uint32_t i = (uint32_t)0U; i < rrLen; i = i + (uint32_t)1U)
  {
    uint64_t src_i = tmp_[i];
    a[i] = src_i;
  }
}

static void
Hacl_Impl_Exponentiation_mul_mod_mont(
  Prims_nat rLen,
  uint32_t iLen,
  uint32_t exp_r,
  uint32_t rrLen,
  uint64_t *st_mont,
  uint64_t *st_kara,
  uint64_t *aM,
  uint64_t *bM,
  uint64_t *resM
)
{
  (void)((uint32_t)8U * iLen);
  uint32_t cLen = rrLen + rrLen;
  uint64_t *c = st_kara;
  uint64_t *st = st_kara + cLen;
  Hacl_Impl_Multiplication_karatsuba(rLen, iLen, rrLen, aM, bM, st_kara);
  Hacl_Impl_Montgomery_mont_reduction(rLen,
    FStar_UInt32_v(cLen),
    exp_r,
    rrLen,
    st_mont,
    st,
    cLen,
    c,
    resM);
}

static void
Hacl_Impl_Exponentiation_mod_exp_(
  Prims_nat rLen,
  Prims_nat bLen,
  uint32_t iLen,
  uint32_t exp_r,
  uint32_t rrLen,
  uint64_t *st_mont,
  uint64_t *st_exp,
  uint64_t *st_kara,
  uint32_t bBits,
  uint32_t bbLen,
  uint64_t *b,
  uint32_t count1
)
{
  uint64_t *aM = st_exp;
  uint64_t *accM = st_exp + rrLen;
  if (count1 < bBits)
  {
    bool uu____1046 = Hacl_Impl_Lib_bn_is_bit_set(bLen, bbLen, b, count1);
    if (uu____1046)
      Hacl_Impl_Exponentiation_mul_mod_mont(rLen,
        iLen,
        exp_r,
        rrLen,
        st_mont,
        st_kara,
        aM,
        accM,
        accM);
    Hacl_Impl_Exponentiation_mul_mod_mont(rLen, iLen, exp_r, rrLen, st_mont, st_kara, aM, aM, aM);
    Hacl_Impl_Exponentiation_mod_exp_(rLen,
      bLen,
      iLen,
      exp_r,
      rrLen,
      st_mont,
      st_exp,
      st_kara,
      bBits,
      bbLen,
      b,
      count1 + (uint32_t)1U);
  }
}

static void
Hacl_Impl_Exponentiation_mod_exp_mont(
  Prims_nat rLen,
  Prims_nat stLen,
  uint32_t iLen,
  uint32_t bBits,
  uint64_t *b,
  uint32_t exp_r,
  uint32_t rrLen,
  uint32_t sstLen,
  uint64_t *st
)
{
  uint32_t bLen = (bBits - (uint32_t)1U) / (uint32_t)64U + (uint32_t)1U;
  (void)(st + (uint32_t)2U * rrLen);
  uint64_t *r2 = st + (uint32_t)3U * rrLen;
  uint64_t *a1 = st + (uint32_t)4U * rrLen;
  uint64_t *acc = st + (uint32_t)5U * rrLen;
  uint64_t *aM = st + (uint32_t)6U * rrLen;
  uint64_t *accM = st + (uint32_t)7U * rrLen;
  uint64_t *res1 = st + (uint32_t)8U * rrLen;
  uint64_t *st_mont = st;
  uint64_t *st_exp = st + (uint32_t)6U * rrLen;
  uint64_t *st_kara = st + (uint32_t)9U * rrLen;
  uint64_t *st_t = st + (uint32_t)9U * rrLen;
  uint64_t *st_f = st + (uint32_t)9U * rrLen;
  Hacl_Impl_Montgomery_to_mont(rLen, exp_r, rrLen, st_mont, st_t, r2, a1, aM);
  Hacl_Impl_Montgomery_to_mont(rLen, exp_r, rrLen, st_mont, st_t, r2, acc, accM);
  Hacl_Impl_Exponentiation_mod_exp_(rLen,
    FStar_UInt32_v((bBits - (uint32_t)1U) / (uint32_t)64U + (uint32_t)1U),
    iLen,
    exp_r,
    rrLen,
    st_mont,
    st_exp,
    st_kara,
    bBits,
    bLen,
    b,
    (uint32_t)0U);
  Hacl_Impl_Montgomery_from_mont(rLen, exp_r, rrLen, st_mont, st_f, accM, res1);
}

static void
Hacl_Impl_Exponentiation_mod_exp(
  Prims_nat nLen,
  uint32_t iLen,
  uint32_t modBits,
  uint32_t nnLen,
  uint64_t *n1,
  uint64_t *a,
  uint32_t bBits,
  uint64_t *b,
  uint64_t *res
)
{
  uint32_t exp_r = modBits + (uint32_t)2U;
  uint32_t rBits = modBits + (uint32_t)3U;
  uint32_t rLen = (rBits - (uint32_t)1U) / (uint32_t)64U + (uint32_t)1U;
  uint32_t stLenKara = (uint32_t)8U * iLen;
  uint32_t stLen_ = (uint32_t)9U * rLen;
  uint32_t stLen = stLenKara + stLen_;
  (void)((uint32_t)3U * rLen);
  uint32_t exp2 = exp_r + exp_r;
  KRML_CHECK_SIZE((uint64_t)0U, stLen);
  uint64_t buf[stLen];
  memset(buf, 0U, stLen * sizeof buf[0U]);
  uint64_t *r = buf;
  uint64_t *n11 = buf + rLen;
  uint64_t *nInv = buf + (uint32_t)2U * rLen;
  uint64_t *r2 = buf + (uint32_t)3U * rLen;
  uint64_t *a1 = buf + (uint32_t)4U * rLen;
  uint64_t *acc = buf + (uint32_t)5U * rLen;
  uint64_t *res1 = buf + (uint32_t)8U * rLen;
  uint64_t *n1_ = n11;
  uint64_t *a1_ = a1;
  uint64_t *res1_ = res1;
  for (uint32_t i = (uint32_t)0U; i < nnLen; i = i + (uint32_t)1U)
  {
    uint64_t src_i = n1[i];
    n1_[i] = src_i;
  }
  for (uint32_t i = (uint32_t)0U; i < nnLen; i = i + (uint32_t)1U)
  {
    uint64_t src_i = a[i];
    a1_[i] = src_i;
  }
  acc[0U] = (uint64_t)1U;
  Hacl_Impl_Lib_bn_set_bit(FStar_UInt32_v(rLen), rLen, r, exp_r);
  Hacl_Impl_Montgomery_bn_pow2_mod_n(FStar_UInt32_v(rLen), modBits, rLen, n11, exp2, r2);
  Hacl_Impl_Montgomery_mont_inverse(FStar_UInt32_v(rLen), rLen, n11, exp_r, nInv);
  Hacl_Impl_Exponentiation_mod_exp_mont(FStar_UInt32_v(rLen),
    FStar_UInt32_v(stLen),
    iLen,
    bBits,
    b,
    exp_r,
    rLen,
    stLen,
    buf);
  for (uint32_t i = (uint32_t)0U; i < nnLen; i = i + (uint32_t)1U)
  {
    uint64_t src_i = res1_[i];
    res[i] = src_i;
  }
}

static uint32_t Hacl_Impl_MGF_hLen = (uint32_t)32U;

void Hacl_Impl_MGF_hash_sha256(Prims_nat x0, uint8_t *mHash, uint32_t len, uint8_t *m)
{
  Hacl_SHA2_256_hash(mHash, m, len);
}


static void
Hacl_Impl_MGF_mgf_(
  Prims_nat accLen,
  Prims_nat stLen,
  uint32_t count_max,
  uint8_t *mgfseed,
  uint32_t aaccLen,
  uint32_t sstLen,
  uint8_t *st,
  uint32_t counter
)
{
  if (counter < count_max)
  {
    uint8_t *mgfseed_counter = st;
    uint8_t *c = st + Hacl_Impl_MGF_hLen + (uint32_t)4U;
    uint8_t *mHash = st + Hacl_Impl_MGF_hLen + (uint32_t)8U;
    uint8_t *acc = st + Hacl_Impl_MGF_hLen + (uint32_t)8U + Hacl_Impl_MGF_hLen;
    c[0U] = (uint8_t)(counter >> (uint32_t)24U);
    c[1U] = (uint8_t)(counter >> (uint32_t)16U);
    c[2U] = (uint8_t)(counter >> (uint32_t)8U);
    c[3U] = (uint8_t)counter;
    uint8_t *mgfseed_counter_ = mgfseed_counter + Hacl_Impl_MGF_hLen;
    for (uint32_t i = (uint32_t)0U; i < (uint32_t)4U; i = i + (uint32_t)1U)
    {
      uint8_t src_i = c[i];
      mgfseed_counter_[i] = src_i;
    }
    Hacl_Impl_MGF_hash_sha256(Prims_op_Addition(FStar_UInt32_v(Hacl_Impl_MGF_hLen),
        (krml_checked_int_t)4),
      mHash,
      Hacl_Impl_MGF_hLen + (uint32_t)4U,
      mgfseed_counter);
    uint8_t *acc_ = acc + Hacl_Impl_MGF_hLen * counter;
    for (uint32_t i = (uint32_t)0U; i < Hacl_Impl_MGF_hLen; i = i + (uint32_t)1U)
    {
      uint8_t src_i = mHash[i];
      acc_[i] = src_i;
    }
    Hacl_Impl_MGF_mgf_(accLen,
      stLen,
      count_max,
      mgfseed,
      aaccLen,
      sstLen,
      st,
      counter + (uint32_t)1U);
  }
}

static void
Hacl_Impl_MGF_mgf_sha256(Prims_nat len, uint8_t *mgfseed, uint32_t clen, uint8_t *res)
{
  uint32_t count_max = (clen - (uint32_t)1U) / Hacl_Impl_MGF_hLen + (uint32_t)1U;
  uint32_t accLen = Hacl_Impl_MGF_hLen * count_max;
  uint32_t stLen = Hacl_Impl_MGF_hLen + (uint32_t)8U + Hacl_Impl_MGF_hLen + accLen;
  KRML_CHECK_SIZE((uint8_t)0U, stLen);
  uint8_t buf[stLen];
  memset(buf, 0U, stLen * sizeof buf[0U]);
  uint8_t *mgfseed_counter = buf;
  uint8_t *acc = buf + Hacl_Impl_MGF_hLen + (uint32_t)8U + Hacl_Impl_MGF_hLen;
  uint8_t *mgfseed_counter_ = mgfseed_counter;
  for (uint32_t i = (uint32_t)0U; i < Hacl_Impl_MGF_hLen; i = i + (uint32_t)1U)
  {
    uint8_t src_i = mgfseed[i];
    mgfseed_counter_[i] = src_i;
  }
  Hacl_Impl_MGF_mgf_(FStar_UInt32_v(accLen),
    FStar_UInt32_v(stLen),
    count_max,
    mgfseed,
    accLen,
    stLen,
    buf,
    (uint32_t)0U);
  uint8_t *acc_ = acc;
  for (uint32_t i = (uint32_t)0U; i < clen; i = i + (uint32_t)1U)
  {
    uint8_t src_i = acc_[i];
    res[i] = src_i;
  }
}

static void Hacl_Impl_RSA_xor_bytes(Prims_nat len, uint32_t clen, uint8_t *b1, uint8_t *b2)
{
  for (uint32_t i = (uint32_t)0U; i < clen; i = i + (uint32_t)1U)
  {
    uint8_t i1 = b1[i];
    uint8_t i2 = b2[i];
    b1[i] = i1 ^ i2;
  }
}

static void
Hacl_Impl_RSA_pss_encode_(
  Prims_nat sLen,
  Prims_nat msgLen,
  Prims_nat emLen,
  uint32_t ssLen,
  uint8_t *salt,
  uint32_t mmsgLen,
  uint8_t *msg,
  uint32_t eemLen,
  uint8_t *em
)
{
  uint32_t m1_size = (uint32_t)40U + ssLen;
  uint32_t db_size = eemLen - (uint32_t)32U - (uint32_t)1U;
  uint32_t stLen = (uint32_t)32U + m1_size + (uint32_t)32U + db_size + db_size;
  KRML_CHECK_SIZE((uint8_t)0U, stLen);
  uint8_t buf[stLen];
  memset(buf, 0U, stLen * sizeof buf[0U]);
  (void)FStar_UInt32_v(stLen);
  uint8_t *mHash = buf;
  Hacl_Impl_MGF_hash_sha256(msgLen, mHash, mmsgLen, msg);
  uint8_t *m1 = buf + (uint32_t)32U;
  uint8_t *m1_ = m1 + (uint32_t)8U;
  for (uint32_t i = (uint32_t)0U; i < (uint32_t)32U; i = i + (uint32_t)1U)
  {
    uint8_t src_i = mHash[i];
    m1_[i] = src_i;
  }
  uint8_t *m1_1 = m1 + (uint32_t)40U;
  for (uint32_t i = (uint32_t)0U; i < ssLen; i = i + (uint32_t)1U)
  {
    uint8_t src_i = salt[i];
    m1_1[i] = src_i;
  }
  uint8_t *m1Hash0 = buf + (uint32_t)32U + m1_size;
  Hacl_Impl_MGF_hash_sha256(FStar_UInt32_v(m1_size), m1Hash0, m1_size, m1);
  uint8_t *db = buf + (uint32_t)32U + m1_size + (uint32_t)32U;
  uint32_t last_before_salt = db_size - ssLen - (uint32_t)1U;
  db[last_before_salt] = (uint8_t)1U;
  uint8_t *db_ = db + last_before_salt + (uint32_t)1U;
  for (uint32_t i = (uint32_t)0U; i < ssLen; i = i + (uint32_t)1U)
  {
    uint8_t src_i = salt[i];
    db_[i] = src_i;
  }
  uint8_t *dbMask = buf + (uint32_t)32U + m1_size + (uint32_t)32U + db_size;
  Hacl_Impl_MGF_mgf_sha256(FStar_UInt32_v(db_size), m1Hash0, db_size, dbMask);
  Hacl_Impl_RSA_xor_bytes(FStar_UInt32_v(db_size), db_size, db, dbMask);
  uint8_t *db0 = buf + (uint32_t)32U + m1_size + (uint32_t)32U;
  uint8_t *em_ = em;
  for (uint32_t i = (uint32_t)0U; i < db_size; i = i + (uint32_t)1U)
  {
    uint8_t src_i = db0[i];
    em_[i] = src_i;
  }
  uint8_t *em_1 = em + db_size;
  uint8_t *m1Hash = buf + (uint32_t)32U + m1_size;
  for (uint32_t i = (uint32_t)0U; i < (uint32_t)32U; i = i + (uint32_t)1U)
  {
    uint8_t src_i = m1Hash[i];
    em_1[i] = src_i;
  }
  em[eemLen - (uint32_t)1U] = (uint8_t)0xbcU;
}

static void
Hacl_Impl_RSA_pss_encode(
  Prims_nat sLen,
  Prims_nat msgLen,
  Prims_nat emLen,
  uint32_t msBits,
  uint32_t ssLen,
  uint8_t *salt,
  uint32_t mmsgLen,
  uint8_t *msg,
  uint32_t eemLen,
  uint8_t *em
)
{
  if (msBits == (uint32_t)0U)
  {
    uint8_t *em_ = em + (uint32_t)1U;
    Hacl_Impl_RSA_pss_encode_(sLen,
      msgLen,
      Prims_op_Subtraction(emLen, (krml_checked_int_t)1),
      ssLen,
      salt,
      mmsgLen,
      msg,
      eemLen - (uint32_t)1U,
      em_);
  }
  else
  {
    Hacl_Impl_RSA_pss_encode_(sLen, msgLen, emLen, ssLen, salt, mmsgLen, msg, eemLen, em);
    uint32_t shift_ = (uint32_t)8U - msBits;
    uint8_t uu____2574 = em[0U];
    uint8_t uu____2573 = uu____2574 & (uint8_t)0xffU >> shift_;
    em[0U] = uu____2573;
  }
}

static bool
Hacl_Impl_RSA_pss_verify_(
  Prims_nat sLen,
  Prims_nat msgLen,
  Prims_nat emLen,
  uint32_t ssLen,
  uint32_t msBits,
  uint32_t eemLen,
  uint8_t *em,
  uint32_t mmsgLen,
  uint8_t *msg
)
{
  uint32_t pad_size = eemLen - ssLen - (uint32_t)32U - (uint32_t)1U;
  uint32_t db_size = eemLen - (uint32_t)32U - (uint32_t)1U;
  uint32_t m1_size = (uint32_t)40U + ssLen;
  uint32_t stLen = (uint32_t)32U + pad_size + db_size + m1_size + (uint32_t)32U;
  KRML_CHECK_SIZE((uint8_t)0U, stLen);
  uint8_t buf[stLen];
  memset(buf, 0U, stLen * sizeof buf[0U]);
  (void)FStar_UInt32_v(stLen);
  uint8_t *mHash = buf;
  Hacl_Impl_MGF_hash_sha256(msgLen, mHash, mmsgLen, msg);
  uint8_t *pad2 = buf + (uint32_t)32U;
  pad2[pad_size - (uint32_t)1U] = (uint8_t)0x01U;
  uint8_t *maskedDB = em;
  uint8_t *m1Hash = em + db_size;
  uint8_t *dbMask = buf + (uint32_t)32U + pad_size;
  Hacl_Impl_MGF_mgf_sha256(FStar_UInt32_v(db_size), m1Hash, db_size, dbMask);
  Hacl_Impl_RSA_xor_bytes(FStar_UInt32_v(db_size), db_size, dbMask, maskedDB);
  if (msBits > (uint32_t)0U)
  {
    uint32_t shift_ = (uint32_t)8U - msBits;
    uint8_t uu____3419 = dbMask[0U];
    uint8_t uu____3418 = uu____3419 & (uint8_t)0xffU >> shift_;
    dbMask[0U] = uu____3418;
  }
  uint8_t *pad = buf + (uint32_t)32U + pad_size;
  uint8_t *salt = dbMask + pad_size;
  uint8_t *m1 = buf + (uint32_t)32U + pad_size + db_size;
  uint8_t *m1Hash_ = buf + (uint32_t)32U + pad_size + db_size + m1_size;
  bool uu____3594 = Hacl_Impl_Lib_eq_b(FStar_UInt32_v(pad_size), pad_size, pad, pad2);
  bool uu____3593 = !uu____3594;
  bool r;
  if (uu____3593)
    r = false;
  else
  {
    uint8_t *m1_ = m1 + (uint32_t)8U;
    for (uint32_t i = (uint32_t)0U; i < (uint32_t)32U; i = i + (uint32_t)1U)
    {
      uint8_t src_i = mHash[i];
      m1_[i] = src_i;
    }
    uint8_t *m1_1 = m1 + (uint32_t)40U;
    for (uint32_t i = (uint32_t)0U; i < ssLen; i = i + (uint32_t)1U)
    {
      uint8_t src_i = salt[i];
      m1_1[i] = src_i;
    }
    Hacl_Impl_MGF_hash_sha256(FStar_UInt32_v(m1_size), m1Hash_, m1_size, m1);
    r = Hacl_Impl_Lib_eq_b(FStar_UInt32_v((uint32_t)32U), (uint32_t)32U, m1Hash, m1Hash_);
  }
  return r;
}

static bool
Hacl_Impl_RSA_pss_verify(
  Prims_nat sLen,
  Prims_nat msgLen,
  Prims_nat emLen,
  uint32_t ssLen,
  uint32_t msBits,
  uint32_t eemLen,
  uint8_t *em,
  uint32_t mmsgLen,
  uint8_t *msg
)
{
  uint8_t uu____4319 = em[0U];
  uint8_t em_0 = uu____4319 & (uint8_t)0xffU << msBits;
  uint8_t em_last = em[eemLen - (uint32_t)1U];
  if (!(em_0 == (uint8_t)0U && em_last == (uint8_t)0xbcU))
    return false;
  else
  {
    uint32_t eemLen1;
    if (msBits == (uint32_t)0U)
      eemLen1 = eemLen - (uint32_t)1U;
    else
      eemLen1 = eemLen;
    uint8_t *em1;
    if (msBits == (uint32_t)0U)
      em1 = em + (uint32_t)1U;
    else
      em1 = em;
    if (eemLen1 < ssLen + (uint32_t)32U + (uint32_t)2U)
      return false;
    else
      return
        Hacl_Impl_RSA_pss_verify_(sLen,
          msgLen,
          FStar_UInt32_v(eemLen1),
          ssLen,
          msBits,
          eemLen1,
          em1,
          mmsgLen,
          msg);
  }
}

void
Hacl_RSAPSS_rsa_pss_sign(
  Prims_nat sLen,
  Prims_nat msgLen,
  uint32_t iLen,
  uint32_t modBits,
  uint32_t eBits,
  uint32_t dBits,
  uint64_t *skey,
  uint32_t ssLen,
  uint8_t *salt,
  uint32_t mmsgLen,
  uint8_t *msg,
  uint8_t *sgnt
)
{
  uint32_t nLen = (modBits - (uint32_t)1U) / (uint32_t)64U + (uint32_t)1U;
  uint32_t eLen = (eBits - (uint32_t)1U) / (uint32_t)64U + (uint32_t)1U;
  uint32_t dLen = (dBits - (uint32_t)1U) / (uint32_t)64U + (uint32_t)1U;
  (void)(nLen + eLen + dLen);
  uint32_t k = (modBits - (uint32_t)1U) / (uint32_t)8U + (uint32_t)1U;
  uint32_t msBits = (modBits - (uint32_t)1U) % (uint32_t)8U;
  uint32_t n2Len = nLen + nLen;
  KRML_CHECK_SIZE((uint8_t)0U, k);
  uint8_t buf[k];
  memset(buf, 0U, k * sizeof buf[0U]);
  Hacl_Impl_RSA_pss_encode(sLen,
    msgLen,
    FStar_UInt32_v(k),
    msBits,
    ssLen,
    salt,
    mmsgLen,
    msg,
    k,
    buf);
  KRML_CHECK_SIZE((uint64_t)0U, n2Len);
  uint64_t buf1[n2Len];
  memset(buf1, 0U, n2Len * sizeof buf1[0U]);
  uint64_t *n1 = skey;
  uint64_t *d = skey + nLen + eLen;
  uint64_t *m = buf1;
  uint64_t *s = buf1 + nLen;
  Hacl_Impl_Convert_text_to_nat(FStar_UInt32_v(k), k, buf, m);
  Hacl_Impl_Exponentiation_mod_exp(FStar_UInt32_v(nLen),
    iLen,
    modBits,
    nLen,
    n1,
    m,
    dBits,
    d,
    s);
  Hacl_Impl_Convert_nat_to_text(FStar_UInt32_v((modBits - (uint32_t)1U)
      / (uint32_t)8U
      + (uint32_t)1U),
    k,
    s,
    sgnt);
}

bool
Hacl_RSAPSS_rsa_pss_verify(
  Prims_nat sLen,
  Prims_nat msgLen,
  uint32_t iLen,
  uint32_t modBits,
  uint32_t eBits,
  uint64_t *pkey,
  uint32_t ssLen,
  uint8_t *sgnt,
  uint32_t mmsgLen,
  uint8_t *msg
)
{
  uint32_t nLen = (modBits - (uint32_t)1U) / (uint32_t)64U + (uint32_t)1U;
  uint32_t eLen = (eBits - (uint32_t)1U) / (uint32_t)64U + (uint32_t)1U;
  (void)(nLen + eLen);
  uint32_t k = (modBits - (uint32_t)1U) / (uint32_t)8U + (uint32_t)1U;
  uint32_t msBits = (modBits - (uint32_t)1U) % (uint32_t)8U;
  uint32_t n2Len = nLen + nLen;
  KRML_CHECK_SIZE((uint64_t)0U, n2Len);
  uint64_t buf[n2Len];
  memset(buf, 0U, n2Len * sizeof buf[0U]);
  uint64_t *s = buf + nLen;
  Hacl_Impl_Convert_text_to_nat(FStar_UInt32_v((modBits - (uint32_t)1U)
      / (uint32_t)8U
      + (uint32_t)1U),
    k,
    sgnt,
    s);
  KRML_CHECK_SIZE((uint8_t)0U, k);
  uint8_t buf1[k];
  memset(buf1, 0U, k * sizeof buf1[0U]);
  uint64_t *m = buf;
  uint64_t *s1 = buf + nLen;
  uint64_t *n1 = pkey;
  uint64_t *e = pkey + nLen;
  bool uu____5831 = Hacl_Impl_Comparison_bn_is_less(FStar_UInt32_v(nLen), nLen, s1, n1);
  bool res;
  if (uu____5831)
  {
    Hacl_Impl_Exponentiation_mod_exp(FStar_UInt32_v(nLen),
      iLen,
      modBits,
      nLen,
      n1,
      s1,
      eBits,
      e,
      m);
    Hacl_Impl_Convert_nat_to_text(FStar_UInt32_v(k), k, m, buf1);
    res =
      Hacl_Impl_RSA_pss_verify(sLen,
        msgLen,
        FStar_UInt32_v(k),
        ssLen,
        msBits,
        k,
        buf1,
        mmsgLen,
        msg);
  }
  else
    res = false;
  return res;
}

