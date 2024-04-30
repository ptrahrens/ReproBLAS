################################################################################
# vectorizations.py                                                            #
#                                                                              #
#     A set of classes used to generate generically vectorized code. Currently #
# supports SISD, Intel SSE, Intel AVX.                                         #
#                                                                              #
#                                                            Willow Ahrens 2014 #
################################################################################


import math
from utils import *
from dataTypes import *

class Vectorization(object):
  name = ""
  defined_macro = ""
  header = None

  def __init__(self, code_block, data_type_class):
    self.code_block = code_block
    self.data_type = data_type_class(code_block)

  def define_unroll_step(self, fold, unroll):
    code_block.write("#define " + self.unroll_step_string(fold) + " " + str(unroll))

  def define_max_unroll_step(self, fold, max_unroll_width):
    code_block.write("#define MAX_" + self.unroll_step_string(fold) + " " + str(max_unroll_width))

  def unroll_step_string(self, fold):
    if fold == -1:
      return "UNROLL_STEP_" + self.name
    else:
      return "UNROLL_STEP_" + self.name + "_" + str(fold) + "-FOLD"

  def consolidate_into(self, dst_ptr, offset, inc, src_vars, common_summand_ptr, common_summand_offset, common_summand_inc):
    raise(NotImplementedError())

  #propagates the value pointed to by pointer to the variables listed in variables
  def propagate_into(self, dst_vars, src_ptr, offset, inc):
    raise(NotImplementedError())

  def add_blp_into(dst, src, blp, width):
    raise(NotImplementedError())

  #loads n values from pointer into the variables listed in variables (n must be a multiple of type_size)
  def load(self, src_ptr, offset, inc, n, align=False):
    raise(NotImplementedError())

  #loads less than type_size values from pointer into the variables listed in variables
  def load_partial(self, src_ptr, offset, inc, n):
    raise(NotImplementedError())

  def sub(self, src_vars, amt_vars):
    raise(NotImplementedError())

  def add(self, src_vars, amt_vars):
    raise(NotImplementedError())

  def mul(self, src_vars, amt_vars):
    raise(NotImplementedError())

  def div(self, src_vars, amt_vars):
    raise(NotImplementedError())

  def max(self, src1_vars, src2_vars):
    raise(NotImplementedError())

  def abs(self, src_vars):
    raise(NotImplementedError())

  def conj(self, src_vars):
    raise(NotImplementedError())

  def set(self, src_var):
    raise(NotImplementedError())

  def set_real_imag(self, real_src_var, imag_src_var):
    raise(NotImplementedError())

  def rep_evens(self, src_vars):
    raise(NotImplementedError())

  def rep_odds(self, src_vars):
    raise(NotImplementedError())

  def swap_pairwise(self, src_vars):
    raise(NotImplementedError())

  def iterate_unrolled(self, i_var, n_var, src_ptrs, src_incs, max_unroll, min_unroll, body):
    i = 0
    unroll = (max_unroll // max(self.type_size, 1)) * max(self.type_size, 1)
    while(unroll >= min_unroll):
      if i == 0:
        self.code_block.write("for({0} = 0; {0} + {1} <= {2}; {0} += {1}, {3}){{".format(i_var, max_unroll, n_var, self.data_type.data_increment(src_ptrs, src_incs, max_unroll)))
        self.code_block.indent()
        body(unroll)
        self.code_block.dedent()
        self.code_block.write("}")
      elif unroll < self.type_size:
        self.code_block.write("if({0} < {1}){{".format(i_var, n_var))
        self.code_block.indent()
        body("({0} - {1})".format(n_var, i_var))
        self.code_block.write("{0};".format(self.data_type.data_increment(src_ptrs, src_incs, "({0} - {1})".format(n_var, i_var))))
        self.code_block.dedent()
        self.code_block.write("}")
        break
      else:
        self.code_block.write("if({0} + {1} <= {2}){{".format(i_var, unroll, n_var))
        self.code_block.indent()
        body(unroll)
        self.code_block.write("{0} += {1}, {2};".format(i_var, unroll, self.data_type.data_increment(src_ptrs, src_incs, unroll)))
        self.code_block.dedent()
        self.code_block.write("}")
      if math.log(unroll, 2) % 1 != 0:
        unroll = 2**int(math.log(unroll, 2))
      else:
        unroll //= 2
      i += 1;

  def set_SIMD_daz_ftz(self):
    raise(NotImplementedError())

  def reset_SIMD_daz_ftz(self):
    raise(NotImplementedError())

class SISD(Vectorization):
  name = "SISD"

  def __init__(self, code_block, data_type_class):
    super(SISD, self).__init__(code_block, data_type_class)
    self.bit_size = self.data_type.base_type.bit_size
    self.byte_size = self.data_type.base_type.byte_size
    self.type_name = self.data_type.base_type.name
    self.base_size = self.bit_size//self.data_type.base_type.bit_size
    self.type_size = self.bit_size//self.data_type.bit_size #notice that type_size is 0 in SISD vectorization if the data type is complex. It is probably better not to use type_size for SISD purposes.
    self.zero = 0
    self.suf_width = self.data_type.base_size

  def include_consolidation_vars(self):
    self.code_block.include("{} cons_tmp; (void)cons_tmp;".format(self.type_name))

  def consolidate_into(self, dst_ptr, offset, inc, src_vars, common_summand_ptr, common_summand_offset, common_summand_inc):
    self.include_consolidation_vars()

    if self.data_type.is_complex:
      dst_ptr = "(({0}*){1})".format(self.data_type.base_type.name, dst_ptr)
      common_summand_ptr = "(({0}*){1})".format(self.data_type.base_type.name, common_summand_ptr)

    if self.data_type.is_complex:
      if(len(src_vars) > 2):
        self.code_block.write("cons_tmp = {0}[{1}];".format(common_summand_ptr, self.data_type.index(common_summand_offset, common_summand_inc, 0)))
      for src_var in src_vars[2::2]:
        self.code_block.write("{0} = {0} + ({1} - cons_tmp);".format(src_vars[0], src_var))
      if(len(src_vars) > 2):
        self.code_block.write("cons_tmp = {0}[{1}];".format(common_summand_ptr, self.data_type.index(common_summand_offset, common_summand_inc, 1)))
      for src_var in src_vars[3::2]:
        self.code_block.write("{0} = {0} + ({1} - cons_tmp);".format(src_vars[1], src_var))
      self.code_block.write("{0}[{1}] = {2};".format(dst_ptr, self.data_type.index(offset, inc, 0), src_vars[0]))
      self.code_block.write("{0}[{1}] = {2};".format(dst_ptr, self.data_type.index(offset, inc, 1), src_vars[1]))
    else:
      if(len(src_vars) > 1):
        self.code_block.write("cons_tmp = {0}[{1}];".format(common_summand_ptr, self.data_type.index(common_summand_offset, common_summand_inc, 0)))
      for src_var in src_vars[1:]:
        self.code_block.write("{0} = {0} + ({1} - cons_tmp);".format(src_vars[0], src_var))
      self.code_block.write("{0}[{1}] = {2};".format(dst_ptr, self.data_type.index(offset, inc, 0), src_vars[0]))

  def max_into(self, dst_ptr, offset, inc, src_vars):
    if self.data_type.is_complex:
      dst_ptr = "(({0}*){1})".format(self.data_type.base_type.name, dst_ptr)

    for i in range(self.data_type.base_size, self.base_size):
      self.code_block.set_equal(src_vars[i % self.data_type.base_size], self.max(src_vars[i % self.data_type.base_size], src_vars[i]))
    if self.data_type.is_complex:
      self.code_block.write("{0}[{1}] = {2};".format(dst_ptr, self.data_type.index(offset, inc, 0), src_vars[0]))
      self.code_block.write("{0}[{1}] = {2};".format(dst_ptr, self.data_type.index(offset, inc, 1), src_vars[1]))
    else:
      self.code_block.write("{0}[{1}] = {2};".format(dst_ptr, self.data_type.index(offset, inc, 0), src_vars[0]))

  def propagate_into(self, dst_vars, src_ptr, offset, inc):
    if self.data_type.is_complex:
      src_ptr = "(({0}*){1})".format(self.data_type.base_type.name, src_ptr)

    if self.data_type.is_complex:
      assert len(dst_vars) % 2 == 0, "cannot propagate complex value to odd number of base type dst_vars"
      self.code_block.write(" = ".join(dst_vars[0::2]) + " = {0}[{1}];".format(src_ptr, self.data_type.index(offset, inc, 0)))
      self.code_block.write(" = ".join(dst_vars[1::2]) + " = {0}[{1}];".format(src_ptr, self.data_type.index(offset, inc, 1)))
    else:
      self.code_block.write(" = ".join(dst_vars) + " = {0}[{1}];".format(src_ptr, self.data_type.index(offset, inc, 0)))

  def include_blp_vars(self):
    self.code_block.include("{0} blp_tmp; (void)blp_tmp;".format(self.data_type.base_type.int_float_name))

  def add_blp_into(self, dst, src, blp, width):
    assert len(dst) >= width
    assert len(src) >= width
    assert len(blp) >= width

    self.include_blp_vars()
    for i in range(width):
      self.code_block.write("blp_tmp.{0} = {1};".format(self.data_type.base_type.float_char, blp[i]))
      self.code_block.write("blp_tmp.{0} |= 1;".format(self.data_type.base_type.int_char))
      self.code_block.write("{0} = {1} + blp_tmp.{2};".format(dst[i], src[i], self.data_type.base_type.float_char))

  def load(self, src_ptr, offset, inc, n, align=False):
    assert n > 0, "n must be nonzero"
    assert inc != 0, "inc must be nonzero"

    if self.data_type.is_complex:
      src_ptr = "(({0}*){1})".format(self.data_type.base_type.name, src_ptr)

    return ["{0}[{1}]".format(src_ptr, self.data_type.index(offset, inc, i)) for i in range(n * self.data_type.base_size)]

  def load_partial(self, src_ptr, offset, inc, n):
    if(isinstance(n, int)):
      assert n > 0, "n must be nonzero"
      assert n < self.type_size, "n must be less than the number of types that fit in a vector"
    assert False, "there is no way to partially fill SISD vectors"

  def set(self, src_var):
    return [src_var]

  def set_real_imag(self, real_src_var, imag_src_var):
    assert self.data_type.is_complex, "cannot set real and imaginary portions of noncomplex"
    return [real_src_var, imag_src_var]

  def sub(self, src_vars, amt_vars):
    return ["({0} - {1})".format(src_var, amt_var) for (src_var, amt_var) in zip(src_vars, amt_vars)]

  def add(self, src_vars, amt_vars):
    return ["({0} + {1})".format(src_var, amt_var) for (src_var, amt_var) in zip(src_vars, amt_vars)]

  def abs(self, src_vars):
    if self.data_type.base_type.name == "double":
      return ["fabs({0})".format(src_var) for src_var in src_vars]
    else:
      return ["fabsf({0})".format(src_var) for src_var in src_vars]

  def mul(self, src_vars, amt_vars):
    return ["({0} * {1})".format(src_var, amt_var) for (src_var, amt_var) in zip(src_vars, amt_vars)]

  def div(self, src_vars, amt_vars):
    return ["({0} / {1})".format(src_var, amt_var) for (src_var, amt_var) in zip(src_vars, amt_vars)]

  def max(self, src1_vars, src2_vars):
    return ["({0} > {1}? {0}: {1})".format(src1_var, src2_var) for (src1_var, src2_var) in zip(src1_vars, src2_vars)]

  def rep_evens(self, src_vars):
    return [src_var for src_var in src_vars[::2] for _ in (0, 1)]

  def rep_odds(self, src_vars):
    return [src_var for src_var in src_vars[1::2] for _ in (0, 1)]

  def swap_pairwise(self, src_vars):
    assert len(src_vars) % 2 == 0, "If you want to swap pairwise, you need an even number of things to swap pairwise."
    return [src_vars[((i + 1) % 2) + (2 * (i // 2))] for i in range(len(src_vars))]

  def conj(self, src_vars):
    if self.data_type.is_complex:
      return [src_var if i % 2 == 0 else self.mul([src_var], ["-1"])[0] for (i, src_var) in enumerate(src_vars)]
    else:
      return src_vars

  def nconj(self, src_vars):
    if self.data_type.is_complex:
      return [src_var if i % 2 == 1 else self.mul([src_var], ["-1"])[0] for (i, src_var) in enumerate(src_vars)]
    else:
      return src_vars

  def set_SIMD_daz_ftz(self):
    pass

  def reset_SIMD_daz_ftz(self):
    pass


class SIMD(Vectorization):

  def __init__(self, code_block, data_type_class):
    super(SIMD, self).__init__(code_block, data_type_class)
    self.suf_width = 1

  def include_max_vars(self):
    self.code_block.include("{0} max_buffer_tmp[{1}] __attribute__((aligned({2}))); (void)max_buffer_tmp;".format(self.data_type.base_type.name, self.base_size, self.byte_size))

  def include_consolidation_vars(self):
    self.code_block.include("{0} cons_tmp; (void)cons_tmp;".format(self.type_name))
    self.code_block.include("{0} cons_buffer_tmp[{1}] __attribute__((aligned({2}))); (void)cons_buffer_tmp;".format(self.data_type.base_type.name, self.base_size, self.byte_size))


class SSE(SIMD):
  name = "SSE"
  defined_macro = "__SSE2__"
  header = "emmintrin.h"

  def __init__(self, code_block, data_type_class):
    super(SSE, self).__init__(code_block, data_type_class)
    self.bit_size = 128
    self.byte_size = 16
    self.type_name = {"float": "__m128", "double": "__m128d"}[self.data_type.base_type.name]
    self.base_size = self.bit_size//self.data_type.base_type.bit_size
    self.type_size = self.bit_size//self.data_type.bit_size
    self.zero = "_mm_setzero_p{0}()".format(self.data_type.base_type.name_char)

  def include_abs_vars(self):
    if self.data_type.base_type.name == "double":
      self.code_block.include("__m128d abs_mask_tmp;\n"
                              "{\n"
                              "  __m128d tmp;\n"
                              "  tmp = _mm_set1_pd(1);\n"
                              "  abs_mask_tmp = _mm_set1_pd(-1);\n"
                              "  abs_mask_tmp = _mm_xor_pd(abs_mask_tmp, tmp);\n"
                              "  tmp = _mm_cmpeq_pd(tmp, tmp);\n"
                              "  abs_mask_tmp = _mm_xor_pd(abs_mask_tmp, tmp);\n"
                              "}")
    else:
      self.code_block.include("__m128 abs_mask_tmp;\n"
                              "{\n"
                              "  __m128 tmp;\n"
                              "  tmp = _mm_set1_ps(1);\n"
                              "  abs_mask_tmp = _mm_set1_ps(-1);\n"
                              "  abs_mask_tmp = _mm_xor_ps(abs_mask_tmp, tmp);\n"
                              "  tmp = _mm_cmpeq_ps(tmp, tmp);\n"
                              "  abs_mask_tmp = _mm_xor_ps(abs_mask_tmp, tmp);\n"
                              "}")

  def include_blp_vars(self):
    if self.data_type.base_type.name == "double":
      self.code_block.include("__m128d blp_mask_tmp;\n"
                              "{\n"
                              "  __m128d tmp;\n"
                              "  blp_mask_tmp = _mm_set1_pd(1.0);\n"
                              "  tmp = _mm_set1_pd(1.0 + (DBL_EPSILON * 1.0001));\n"
                              "  blp_mask_tmp = _mm_xor_pd(blp_mask_tmp, tmp);\n"
                              "}")
    else:
      self.code_block.include("__m128 blp_mask_tmp;\n"
                              "{\n"
                              "  __m128 tmp;\n"
                              "  blp_mask_tmp = _mm_set1_ps(1.0);\n"
                              "  tmp = _mm_set1_ps(1.0 + (FLT_EPSILON * 1.0001));\n"
                              "  blp_mask_tmp = _mm_xor_ps(blp_mask_tmp, tmp);\n"
                              "}")

  def include_conj_vars(self):
    if self.data_type.base_type.name == "double":
      self.code_block.include("__m128d conj_mask_tmp;\n"
                              "{\n"
                              "  __m128d tmp;\n"
                              "  tmp = _mm_set_pd(1, 0);\n"
                              "  conj_mask_tmp = _mm_set_pd(-1, 0);\n"
                              "  conj_mask_tmp = _mm_xor_pd(conj_mask_tmp, tmp);\n"
                              "}")
    else:
      self.code_block.include("__m128 conj_mask_tmp;\n"
                              "{\n"
                              "  __m128 tmp;\n"
                              "  tmp = _mm_set_ps(1, 0, 1, 0);\n"
                              "  conj_mask_tmp = _mm_set_ps(-1, 0, -1, 0);\n"
                              "  conj_mask_tmp = _mm_xor_ps(conj_mask_tmp, tmp);\n"
                              "}")

  def include_nconj_vars(self):
    if self.data_type.base_type.name == "double":
      self.code_block.include("__m128d nconj_mask_tmp;\n"
                              "{\n"
                              "  __m128d tmp;\n"
                              "  tmp = _mm_set_pd(0, 1);\n"
                              "  nconj_mask_tmp = _mm_set_pd(0, -1);\n"
                              "  nconj_mask_tmp = _mm_xor_pd(nconj_mask_tmp, tmp);\n"
                              "}")
    else:
      self.code_block.include("__m128 nconj_mask_tmp;\n"
                              "{\n"
                              "  __m128 tmp;\n"
                              "  tmp = _mm_set_ps(0, 1, 0, 1);\n"
                              "  nconj_mask_tmp = _mm_set_ps(0, -1, 0, -1);\n"
                              "  nconj_mask_tmp = _mm_xor_ps(nconj_mask_tmp, tmp);\n"
                              "}")

  def include_SIMD_daz_ftz_vars(self):
    self.code_block.include("unsigned int SIMD_daz_ftz_old_tmp = 0;")
    self.code_block.include("unsigned int SIMD_daz_ftz_new_tmp = 0;")

  def consolidate_into(self, dst_ptr, offset, inc, src_vars, common_summand_ptr, common_summand_offset, common_summand_inc):
    self.include_consolidation_vars()

    if self.data_type.is_complex:
      dst_ptr = "(({0}*){1})".format(self.data_type.base_type.name, dst_ptr)
      common_summand_ptr = "(({0}*){1})".format(self.data_type.base_type.name, common_summand_ptr)

    if self.data_type.name == "float":
      self.code_block.write("{0} = _mm_sub_ps({0}, _mm_set_ps({1}[{2}], {1}[{2}], {1}[{2}], 0));".format(src_vars[0], common_summand_ptr, self.data_type.index(common_summand_offset, common_summand_inc, 0)))
    elif self.data_type.name == "double":
      self.code_block.write("{0} = _mm_sub_pd({0}, _mm_set_pd({1}[{2}], 0));".format(src_vars[0], common_summand_ptr, self.data_type.index(common_summand_offset, common_summand_inc, 0)))
    elif self.data_type.name == "float complex":
      self.code_block.write("{0} = _mm_sub_ps({0}, _mm_set_ps({1}[{3}], {1}[{2}], 0, 0));".format(src_vars[0], common_summand_ptr, self.data_type.index(common_summand_offset, common_summand_inc, 0), self.data_type.index(common_summand_offset, common_summand_inc, 1)))
    if len(src_vars) > 1:
      self.propagate_into(["cons_tmp"], common_summand_ptr, common_summand_offset, common_summand_inc)
      for src_var in src_vars[1:]:
        self.code_block.write("{0} = _mm_add_p{1}({0}, _mm_sub_p{1}({2}, cons_tmp));".format(src_vars[0], self.data_type.base_type.name_char, src_var))
    self.code_block.write("_mm_store_p{0}(cons_buffer_tmp, {1});".format(self.data_type.base_type.name_char, src_vars[0]))
    if self.data_type.is_complex:
      self.code_block.write("{0}[{1}] = {2};".format(dst_ptr, self.data_type.index(offset, inc, 0), " + ".join(["cons_buffer_tmp[{0}]".format(self.data_type.index(i, 1, 0)) for i in range(self.type_size)])))
      self.code_block.write("{0}[{1}] = {2};".format(dst_ptr, self.data_type.index(offset, inc, 1), " + ".join(["cons_buffer_tmp[{0}]".format(self.data_type.index(i, 1, 1)) for i in range(self.type_size)])))
    else:
      self.code_block.write("{0}[{1}] = {2};".format(dst_ptr, self.data_type.index(offset, inc, 0), " + ".join(["cons_buffer_tmp[{0}]".format(i) for i in range(self.base_size)])))

  def max_into(self, dst_ptr, offset, inc, src_vars):
    self.include_max_vars()

    if self.data_type.is_complex:
      dst_ptr = "(({0}*){1})".format(self.data_type.base_type.name, dst_ptr)

    for src_var in src_vars[1:]:
      self.set_equal(self.max(src_var[0], src_var));
    self.code_block.write("_mm_store_p{0}(max_buffer_tmp, {1});".format(self.data_type.base_type.name_char, src_vars[0]))
    for i in range(self.data_type.base_size, self.base_size):
      self.code_block.write("max_buffer_tmp[{0}] = (max_buffer_tmp[{0}] > max_buffer_tmp[{1}] ? max_buffer_tmp[{0}]: max_buffer_tmp[{1}]);".format(i % self.data_type.base_size, i))
    for i in range(self.data_type.base_size):
      self.code_block.write("{0}[{1}] = max_buffer_tmp[{2}];".format(dst_ptr, self.data_type.index(offset, inc, i, True), i))

  def propagate_into(self, dst_vars, src_ptr, offset, inc):
    if self.data_type.is_complex:
      src_ptr = "(({0}*){1})".format(self.data_type.base_type.name, src_ptr)

    if self.data_type.is_complex:
      broadcast = {"float": "(__m128)_mm_load1_pd((double *)({0}));", "double": "_mm_loadu_pd({0});"}[self.data_type.base_type.name]
    else:
      broadcast = "_mm_load1_p{0}({{0}});".format(self.data_type.name_char)
    self.code_block.write(" = ".join(dst_vars) + " = " + broadcast.format(mix("+", src_ptr, self.data_type.index(offset, inc, 0), paren=False)))

  def add_blp_into(self, dst, src, blp, width):
    assert len(dst) >= width
    assert len(src) >= width
    assert len(blp) >= width

    self.include_blp_vars()
    for i in range(width):
      self.code_block.write("{0} = _mm_add_p{1}({2}, _mm_or_p{1}({3}, blp_mask_tmp));".format(dst[i], self.data_type.base_type.name_char, src[i], blp[i]))

  def load(self, src_ptr, offset, inc, n, align=False):
    assert n > 0, "n must be nonzero"
    assert n % self.type_size == 0, "n must be a multiple of the number of types that fit in a vector"

    if self.data_type.is_complex:
      src_ptr = "(({0}*){1})".format(self.data_type.base_type.name, src_ptr)

    result = []
    for i in range(n//self.type_size):
      if inc == 1 or self.type_size == 1:
        if align:
          result += ["_mm_load_p{0}({1})".format(self.data_type.base_type.name_char, mix("+", src_ptr, self.data_type.index(offset, inc, i * self.base_size), paren=False))]
        else:
          result += ["_mm_loadu_p{0}({1})".format(self.data_type.base_type.name_char, mix("+", src_ptr, self.data_type.index(offset, inc, i * self.base_size), paren=False))]
      else:
        if self.data_type.base_type.name == "float":
          result += ["_mm_set_ps({0}[{4}], {0}[{3}], {0}[{2}], {0}[{1}])".format(src_ptr, self.data_type.index(offset, inc, i * self.base_size), self.data_type.index(offset, inc, i * self.base_size + 1), self.data_type.index(offset, inc, i * self.base_size + 2), self.data_type.index(offset, inc, i * self.base_size + 3))]
        if self.data_type.base_type.name == "double":
          result += ["_mm_set_pd({0}[{2}], {0}[{1}])".format(src_ptr, self.data_type.index(offset, inc, i * self.base_size), self.data_type.index(offset, inc, i * self.base_size + 1))]
    return result

  def load_partial(self, src_ptr, offset, inc, n):
    if self.data_type.is_complex:
      src_ptr = "(({0}*){1})".format(self.data_type.base_type.name, src_ptr)

    if(isinstance(n, int)):
      assert n > 0, "n must be nonzero"
      assert n < self.type_size, "n must be less than the number of types that fit in a vector"
    if self.data_type.name == "float complex":
      return ["_mm_set_ps(0, 0, {0}[{2}], {0}[{1}])".format(src_ptr, self.data_type.index(offset, inc, 0), self.data_type.index(offset, inc, 1))]
    elif self.data_type.name == "double":
      return ["_mm_set_pd(0, {0}[{1}])".format(src_ptr, self.data_type.index(offset, inc, 0))]
    elif self.data_type.name == "float":
      return ["_mm_set_ps(0, {1}>2?{0}[{4}]:0, {1}>1?{0}[{3}]:0, {0}[{2}])".format(src_ptr, n, self.data_type.index(offset, inc, 0), self.data_type.index(offset, inc, 1), self.data_type.index(offset, inc, 2))]

  def sub(self, src_vars, amt_vars):
    return ["_mm_sub_p{0}({1}, {2})".format(self.data_type.base_type.name_char, src_var, amt_var) for (src_var, amt_var) in zip(src_vars, amt_vars)]

  def add(self, src_vars, amt_vars):
    return ["_mm_add_p{0}({1}, {2})".format(self.data_type.base_type.name_char, src_var, amt_var) for (src_var, amt_var) in zip(src_vars, amt_vars)]

  def abs(self, src_vars):
    self.include_abs_vars()
    return ["_mm_and_p{0}({1}, abs_mask_tmp)".format(self.data_type.base_type.name_char, src_var) for src_var in src_vars]

  def mul(self, src_vars, amt_vars):
    return ["_mm_mul_p{0}({1}, {2})".format(self.data_type.base_type.name_char, src_var, amt_var) for (src_var, amt_var) in zip(src_vars, amt_vars)]

  def div(self, src_vars, amt_vars):
    return ["_mm_div_p{0}({1}, {2})".format(self.data_type.base_type.name_char, src_var, amt_var) for (src_var, amt_var) in zip(src_vars, amt_vars)]

  def max(self, src1_vars, src2_vars):
    return ["_mm_max_p{0}({1}, {2})".format(self.data_type.base_type.name_char, src1_var, src2_var) for (src1_var, src2_var) in zip(src1_vars, src2_vars)]

  def conj(self, src_vars):
    if self.data_type.is_complex:
      self.include_conj_vars()
      return ["_mm_xor_p{0}({1}, conj_mask_tmp)".format(self.data_type.base_type.name_char, src_var) for src_var in src_vars]
    else:
      return src_vars

  def nconj(self, src_vars):
    if self.data_type.is_complex:
      self.include_nconj_vars()
      return ["_mm_xor_p{0}({1}, nconj_mask_tmp)".format(self.data_type.base_type.name_char, src_var) for src_var in src_vars]
    else:
      return src_vars

  def set(self, src_var):
    return ["_mm_set1_p{0}({1})".format(self.data_type.base_type.name_char, src_var)]

  def set_real_imag(self, real_src_var, imag_src_var):
    assert self.data_type.is_complex, "cannot set real and imaginary portions of noncomplex"
    if self.data_type.name == "double complex":
      return ["_mm_set_pd({1}, {0})".format(real_src_var, imag_src_var)]
    elif self.data_type.name == "float complex":
      return ["_mm_set_ps({1}, {0}, {1}, {0})".format(real_src_var, imag_src_var)]

  def rep_evens(self, src_vars):
    if self.data_type.base_type.name == "double":
      return ["_mm_shuffle_pd({0}, {0}, 0x0)".format(src_var) for src_var in src_vars]
    elif self.data_type.base_type.name == "float":
      return ["_mm_shuffle_ps({0}, {0}, 0xA0)".format(src_var) for src_var in src_vars]

  def rep_odds(self, src_vars):
    if self.data_type.base_type.name == "double":
      return ["_mm_shuffle_pd({0}, {0}, 0x3)".format(src_var) for src_var in src_vars]
    elif self.data_type.base_type.name == "float":
      return ["_mm_shuffle_ps({0}, {0}, 0xF5)".format(src_var) for src_var in src_vars]

  def swap_pairwise(self, src_vars):
    if self.data_type.base_type.name == "double":
      return ["_mm_shuffle_pd({0}, {0}, 0x1)".format(src_var) for src_var in src_vars]
    elif self.data_type.base_type.name == "float":
      return ["_mm_shuffle_ps({0}, {0}, 0xB1)".format(src_var) for src_var in src_vars]

  def set_SIMD_daz_ftz(self):
    self.include_SIMD_daz_ftz_vars();
    self.code_block.write("SIMD_daz_ftz_old_tmp = _mm_getcsr();\n"
                          "SIMD_daz_ftz_new_tmp = SIMD_daz_ftz_old_tmp | 0x8040;\n"
                          "if(SIMD_daz_ftz_new_tmp != SIMD_daz_ftz_old_tmp){\n"
                          "  _mm_setcsr(SIMD_daz_ftz_new_tmp);\n"
                          "}")

  def reset_SIMD_daz_ftz(self):
    self.include_SIMD_daz_ftz_vars();
    self.code_block.write("if(SIMD_daz_ftz_new_tmp != SIMD_daz_ftz_old_tmp){\n"
                          "  _mm_setcsr(SIMD_daz_ftz_old_tmp);\n"
                          "}")


class AVX(SIMD):
  name = "AVX"
  defined_macro = "__AVX__"
  header = "immintrin.h"

  def __init__(self, code_block, data_type_class):
    super(AVX, self).__init__(code_block, data_type_class)
    self.bit_size = 256
    self.byte_size = 32
    self.type_name = {"float": "__m256", "double": "__m256d"}[self.data_type.base_type.name]
    self.base_size = self.bit_size//self.data_type.base_type.bit_size
    self.type_size = self.bit_size//self.data_type.bit_size
    self.zero = "_mm256_setzero_p{0}()".format(self.data_type.base_type.name_char)

  def include_abs_vars(self):
    if self.data_type.base_type.name == "double":
      self.code_block.include("__m256d abs_mask_tmp;\n"
                              "{\n"
                              "  __m256d tmp;\n"
                              "  tmp = _mm256_set1_pd(1);\n"
                              "  abs_mask_tmp = _mm256_set1_pd(-1);\n"
                              "  abs_mask_tmp = _mm256_xor_pd(abs_mask_tmp, tmp);\n"
                              "  tmp = _mm256_cmp_pd(tmp, tmp, 0);\n"
                              "  abs_mask_tmp = _mm256_xor_pd(abs_mask_tmp, tmp);\n"
                              "}")
    else:
      self.code_block.include("__m256 abs_mask_tmp;\n"
                              "{\n"
                              "  __m256 tmp;\n"
                              "  tmp = _mm256_set1_ps(1);\n"
                              "  abs_mask_tmp = _mm256_set1_ps(-1);\n"
                              "  abs_mask_tmp = _mm256_xor_ps(abs_mask_tmp, tmp);\n"
                              "  tmp = _mm256_cmp_ps(tmp, tmp, 0);\n"
                              "  abs_mask_tmp = _mm256_xor_ps(abs_mask_tmp, tmp);\n"
                              "}")

  def include_blp_vars(self):
    if self.data_type.base_type.name == "double":
      self.code_block.include("__m256d blp_mask_tmp;\n"
                              "{\n"
                              "  __m256d tmp;\n"
                              "  blp_mask_tmp = _mm256_set1_pd(1.0);\n"
                              "  tmp = _mm256_set1_pd(1.0 + (DBL_EPSILON * 1.0001));\n"
                              "  blp_mask_tmp = _mm256_xor_pd(blp_mask_tmp, tmp);\n"
                              "}")
    else:
      self.code_block.include("__m256 blp_mask_tmp;\n"
                              "{\n"
                              "  __m256 tmp;\n"
                              "  blp_mask_tmp = _mm256_set1_ps(1.0);\n"
                              "  tmp = _mm256_set1_ps(1.0 + (FLT_EPSILON * 1.0001));\n"
                              "  blp_mask_tmp = _mm256_xor_ps(blp_mask_tmp, tmp);\n"
                              "}")

  def include_conj_vars(self):
    if self.data_type.base_type.name == "double":
      self.code_block.include("__m256d conj_mask_tmp;\n"
                              "{\n"
                              "  __m256d tmp;\n"
                              "  tmp = _mm256_set_pd(1, 0, 1, 0);\n"
                              "  conj_mask_tmp = _mm256_set_pd(-1, 0, -1, 0);\n"
                              "  conj_mask_tmp = _mm256_xor_pd(conj_mask_tmp, tmp);\n"
                              "}")
    else:
      self.code_block.include("__m256 conj_mask_tmp;\n"
                              "{\n"
                              "  __m256 tmp;\n"
                              "  tmp = _mm256_set_ps(1, 0, 1, 0, 1, 0, 1, 0);\n"
                              "  conj_mask_tmp = _mm256_set_ps(-1, 0, -1, 0, -1, 0, -1, 0);\n"
                              "  conj_mask_tmp = _mm256_xor_ps(conj_mask_tmp, tmp);\n"
                              "}")

  def include_nconj_vars(self):
    if self.data_type.base_type.name == "double":
      self.code_block.include("__m256d nconj_mask_tmp;\n"
                              "{\n"
                              "  __m256d tmp;\n"
                              "  tmp = _mm256_set_pd(0, 1, 0, 1);\n"
                              "  nconj_mask_tmp = _mm256_set_pd(0, -1, 0, -1);\n"
                              "  nconj_mask_tmp = _mm256_xor_pd(nconj_mask_tmp, tmp);\n"
                              "}")
    else:
      self.code_block.include("__m256 nconj_mask_tmp;\n"
                              "{\n"
                              "  __m256 tmp;\n"
                              "  tmp = _mm256_set_ps(0, 1, 0, 1, 0, 1, 0, 1);\n"
                              "  nconj_mask_tmp = _mm256_set_ps(0, -1, 0, -1, 0, -1, 0, -1);\n"
                              "  nconj_mask_tmp = _mm256_xor_ps(nconj_mask_tmp, tmp);\n"
                              "}")

  def include_SIMD_daz_ftz_vars(self):
    self.code_block.include("unsigned int SIMD_daz_ftz_old_tmp = 0;")
    self.code_block.include("unsigned int SIMD_daz_ftz_new_tmp = 0;")

  def consolidate_into(self, dst_ptr, offset, inc, src_vars, common_summand_ptr, common_summand_offset, common_summand_inc):
    self.include_consolidation_vars()

    if self.data_type.is_complex:
      dst_ptr = "(({0}*){1})".format(self.data_type.base_type.name, dst_ptr)
      common_summand_ptr = "(({0}*){1})".format(self.data_type.base_type.name, common_summand_ptr)

    if self.data_type.name == "float":
      self.code_block.write("{0} = _mm256_sub_ps({0}, _mm256_set_ps({1}[{2}], {1}[{2}], {1}[{2}], {1}[{2}], {1}[{2}], {1}[{2}], {1}[{2}], 0));".format(src_vars[0], common_summand_ptr, self.data_type.index(common_summand_offset, common_summand_inc, 0)))
    elif self.data_type.name == "double":
      self.code_block.write("{0} = _mm256_sub_pd({0}, _mm256_set_pd({1}[{2}], {1}[{2}], {1}[{2}], 0));".format(src_vars[0], common_summand_ptr, self.data_type.index(common_summand_offset, common_summand_inc, 0)))
    elif self.data_type.name == "float complex":
      self.code_block.write("{0} = _mm256_sub_ps({0}, _mm256_set_ps({1}[{3}], {1}[{2}], {1}[{3}], {1}[{2}], {1}[{3}], {1}[{2}], 0, 0));".format(src_vars[0], common_summand_ptr, self.data_type.index(common_summand_offset, common_summand_inc, 0), self.data_type.index(common_summand_offset, common_summand_inc, 1)))
    elif self.data_type.name == "double complex":
      self.code_block.write("{0} = _mm256_sub_pd({0}, _mm256_set_pd({1}[{3}], {1}[{2}], 0, 0));".format(src_vars[0], common_summand_ptr, self.data_type.index(common_summand_offset, common_summand_inc, 0), self.data_type.index(common_summand_offset, common_summand_inc, 1)))
    if len(src_vars) > 1:
      self.propagate_into(["cons_tmp"], common_summand_ptr, common_summand_offset, common_summand_inc)
      for src_var in src_vars[1:]:
        self.code_block.write("{0} = _mm256_add_p{1}({0}, _mm256_sub_p{1}({2}, cons_tmp));".format(src_vars[0], self.data_type.base_type.name_char, src_var))
    self.code_block.write("_mm256_store_p{0}(cons_buffer_tmp, {1});".format(self.data_type.base_type.name_char, src_vars[0]))
    if self.data_type.is_complex:
      self.code_block.write("{0}[{1}] = {2};".format(dst_ptr, self.data_type.index(offset, inc, 0), " + ".join(["cons_buffer_tmp[{0}]".format(2 * i) for i in range(self.type_size)])))
      self.code_block.write("{0}[{1}] = {2};".format(dst_ptr, self.data_type.index(offset, inc, 1), " + ".join(["cons_buffer_tmp[{0}]".format(2 * i + 1) for i in range(self.type_size)])))
    else:
      self.code_block.write("{0}[{1}] = {2};".format(dst_ptr, self.data_type.index(offset, inc, 0), " + ".join(["cons_buffer_tmp[{0}]".format(i) for i in range(self.base_size)])))

  def max_into(self, dst_ptr, offset, inc, src_vars): 
    self.include_max_vars()

    if self.data_type.is_complex:
      dst_ptr = "(({0}*){1})".format(self.data_type.base_type.name, dst_ptr)

    for src_var in src_vars[1:]:
      self.set_equal(self.max(src_var[0], src_var));
    self.code_block.write("_mm256_store_p{0}(max_buffer_tmp, {1});".format(self.data_type.base_type.name_char, src_vars[0]))
    for i in range(self.data_type.base_size, self.base_size):
      self.code_block.write("max_buffer_tmp[{0}] = (max_buffer_tmp[{0}] > max_buffer_tmp[{1}] ? max_buffer_tmp[{0}]: max_buffer_tmp[{1}]);".format(i % self.data_type.base_size, i))
    for i in range(self.data_type.base_size):
      self.code_block.write("{0}[{1}] = max_buffer_tmp[{2}];".format(dst_ptr, self.data_type.index(offset, inc, i, True), i))

  def propagate_into(self, dst_vars, src_ptr, offset, inc):
    if self.data_type.is_complex:
      src_ptr = "(({0}*){1})".format(self.data_type.base_type.name, src_ptr)

    if self.data_type.is_complex:
      broadcast = {"float": "(__m256)_mm256_broadcast_sd((double *)({0}));", "double": "_mm256_broadcast_pd((__m128d *)({0}));"}[self.data_type.base_type.name]
    else:
      broadcast = "_mm256_broadcast_s{0}({{0}});".format(self.data_type.name_char)
    self.code_block.write(" = ".join(dst_vars) + " = " + broadcast.format(mix("+", src_ptr, self.data_type.index(offset, inc, 0), paren=False)))

  def add_blp_into(self, dst, src, blp, width):
    assert len(dst) >= width
    assert len(src) >= width
    assert len(blp) >= width
    self.include_blp_vars()
    for i in range(width):
      self.code_block.write("{0} = _mm256_add_p{1}({2}, _mm256_or_p{1}({3}, blp_mask_tmp));".format(dst[i], self.data_type.base_type.name_char, src[i], blp[i]))

  def load(self, src_ptr, offset, inc, n, align=False):
    assert n > 0, "n must be nonzero"
    assert n % self.type_size == 0, "n must be a multiple of the number of types that fit in a vector"

    if self.data_type.is_complex:
      src_ptr = "(({0}*){1})".format(self.data_type.base_type.name, src_ptr)

    result = []
    for i in range(n//self.type_size):
      if inc == 1 or self.type_size == 1:
        if align:
          result += ["_mm256_load_p{0}({1})".format(self.data_type.base_type.name_char, mix("+", src_ptr, self.data_type.index(offset, inc, i * self.base_size), paren=False))]
        else:
          result += ["_mm256_loadu_p{0}({1})".format(self.data_type.base_type.name_char, mix("+", src_ptr, self.data_type.index(offset, inc, i * self.base_size), paren=False))]
      else:
        if self.data_type.base_type.name == "float":
          result += ["_mm256_set_ps({0}[{8}], {0}[{7}], {0}[{6}], {0}[{5}], {0}[{4}], {0}[{3}], {0}[{2}], {0}[{1}])".format(src_ptr, self.data_type.index(offset, inc, i * self.base_size), self.data_type.index(offset, inc, i * self.base_size + 1), self.data_type.index(offset, inc, i * self.base_size + 2), self.data_type.index(offset, inc, i * self.base_size + 3), self.data_type.index(offset, inc, i * self.base_size + 4), self.data_type.index(offset, inc, i * self.base_size + 5), self.data_type.index(offset, inc, i * self.base_size + 6), self.data_type.index(offset, inc, i * self.base_size + 7))]
        elif self.data_type.base_type.name == "double":
          result += ["_mm256_set_pd({0}[{4}], {0}[{3}], {0}[{2}], {0}[{1}])".format(src_ptr, self.data_type.index(offset, inc, i * self.base_size), self.data_type.index(offset, inc, i * self.base_size + 1), self.data_type.index(offset, inc, i * self.base_size + 2), self.data_type.index(offset, inc, i * self.base_size + 3))]
    return result

  def load_partial(self, src_ptr, offset, inc, n):
    if self.data_type.is_complex:
      src_ptr = "(({0}*){1})".format(self.data_type.base_type.name, src_ptr)

    if(isinstance(n, int)):
      assert n > 0, "n must be nonzero"
      assert n < self.type_size, "n must be less than the number of types that fit in a vector"
    if self.data_type.name == "double complex":
      return ["_mm256_set_pd(0, 0, {0}[{2}], {0}[{1}])".format(src_ptr, self.data_type.index(offset, inc, 0), self.data_type.index(offset, inc, 1))]
    elif self.data_type.name == "float complex":
      return ["(__m256)_mm256_set_pd(0, {1}>2?((double*){0})[{4}]:0, {1}>1?((double*){0})[{3}]:0, ((double*){0})[{2}])".format(src_ptr, n, self.data_type.index(offset, inc, 0, False), self.data_type.index(offset, inc, 1, False), self.data_type.index(offset, inc, 2, False))]
    elif self.data_type.name == "double":
      return ["_mm256_set_pd(0, {1}>2?{0}[{4}]:0, {1}>1?{0}[{3}]:0, {0}[{2}])".format(src_ptr, n, self.data_type.index(offset, inc, 0), self.data_type.index(offset, inc, 1), self.data_type.index(offset, inc, 2))]
    elif self.data_type.name == "float":
      return ["_mm256_set_ps(0, {1}>6?{0}[{8}]:0, {1}>5?{0}[{7}]:0, {1}>4?{0}[{6}]:0, {1}>3?{0}[{5}]:0, {1}>2?{0}[{4}]:0, {1}>1?{0}[{3}]:0, {0}[{2}])".format(src_ptr, n, self.data_type.index(offset, inc, 0), self.data_type.index(offset, inc, 1), self.data_type.index(offset, inc, 2), self.data_type.index(offset, inc, 3), self.data_type.index(offset, inc, 4), self.data_type.index(offset, inc, 5), self.data_type.index(offset, inc, 6))]

  def sub(self, src_vars, amt_vars):
    return ["_mm256_sub_p{0}({1}, {2})".format(self.data_type.base_type.name_char, src_var, amt_var) for (src_var, amt_var) in zip(src_vars, amt_vars)]

  def add(self, src_vars, amt_vars):
    return ["_mm256_add_p{0}({1}, {2})".format(self.data_type.base_type.name_char, src_var, amt_var) for (src_var, amt_var) in zip(src_vars, amt_vars)]

  def abs(self, src_vars):
    self.include_abs_vars()
    return ["_mm256_and_p{0}({1}, abs_mask_tmp)".format(self.data_type.base_type.name_char, src_var) for src_var in src_vars]

  def mul(self, src_vars, amt_vars):
    return ["_mm256_mul_p{0}({1}, {2})".format(self.data_type.base_type.name_char, src_var, amt_var) for (src_var, amt_var) in zip(src_vars, amt_vars)]

  def div(self, src_vars, amt_vars):
    return ["_mm256_div_p{0}({1}, {2})".format(self.data_type.base_type.name_char, src_var, amt_var) for (src_var, amt_var) in zip(src_vars, amt_vars)]

  def max(self, src1_vars, src2_vars):
    return ["_mm256_max_p{0}({1}, {2})".format(self.data_type.base_type.name_char, src1_var, src2_var) for (src1_var, src2_var) in zip(src1_vars, src2_vars)]

  def conj(self, src_vars):
    if self.data_type.is_complex:
      self.include_conj_vars()
      return ["_mm256_xor_p{0}({1}, conj_mask_tmp)".format(self.data_type.base_type.name_char, src_var) for src_var in src_vars]
    else:
      return src_vars

  def nconj(self, src_vars):
    if self.data_type.is_complex:
      self.include_nconj_vars()
      return ["_mm256_xor_p{0}({1}, nconj_mask_tmp)".format(self.data_type.base_type.name_char, src_var) for src_var in src_vars]
    else:
      return src_vars

  def set(self, src_var):
    return ["_mm256_set1_p{0}({1})".format(self.data_type.base_type.name_char, src_var)]

  def set_real_imag(self, real_src_var, imag_src_var):
    assert self.data_type.is_complex, "cannot set real and imaginary portions of noncomplex"
    if self.data_type.name == "double complex":
      return ["_mm256_set_pd({1}, {0}, {1}, {0})".format(real_src_var, imag_src_var)]
    elif self.data_type.name == "float complex":
      return ["_mm256_set_ps({1}, {0}, {1}, {0}, {1}, {0}, {1}, {0})".format(real_src_var, imag_src_var)]

  def rep_evens(self, src_vars):
    if self.data_type.base_type.name == "double":
      return ["_mm256_permute_pd({0}, 0x0)".format(src_var) for src_var in src_vars]
    elif self.data_type.base_type.name == "float":
      return ["_mm256_permute_ps({0}, 0xA0)".format(src_var) for src_var in src_vars]

  def rep_odds(self, src_vars):
    if self.data_type.base_type.name == "double":
      return ["_mm256_permute_pd({0}, 0xF)".format(src_var) for src_var in src_vars]
    elif self.data_type.base_type.name == "float":
      return ["_mm256_permute_ps({0}, 0xF5)".format(src_var) for src_var in src_vars]

  def swap_pairwise(self, src_vars):
    if self.data_type.base_type.name == "double":
      return ["_mm256_permute_pd({0}, 0x5)".format(src_var) for src_var in src_vars]
    elif self.data_type.base_type.name == "float":
      return ["_mm256_permute_ps({0}, 0xB1)".format(src_var) for src_var in src_vars]

  def set_SIMD_daz_ftz(self):
    self.include_SIMD_daz_ftz_vars();
    self.code_block.write("SIMD_daz_ftz_old_tmp = _mm_getcsr();\n"
                          "SIMD_daz_ftz_new_tmp = SIMD_daz_ftz_old_tmp | 0x8040;\n"
                          "if(SIMD_daz_ftz_new_tmp != SIMD_daz_ftz_old_tmp){\n"
                          "  _mm_setcsr(SIMD_daz_ftz_new_tmp);\n"
                          "}")

  def reset_SIMD_daz_ftz(self):
    self.include_SIMD_daz_ftz_vars();
    self.code_block.write("if(SIMD_daz_ftz_new_tmp != SIMD_daz_ftz_old_tmp){\n"
                          "  _mm_setcsr(SIMD_daz_ftz_old_tmp);\n"
                          "}")

vectorization_lookup = {"SISD":SISD, "SSE":SSE, "AVX":AVX}

#all_vectorizations = [AVX, SSE, SISD]
all_vectorizations = [AVX, SSE, SISD]

def iterate_all_vectorizations(f, code_block):
  for (i, vectorization) in enumerate(all_vectorizations):
    if i == 0 and len(all_vectorizations) > 1:
      code_block.write("#if (defined({0}) && !defined(reproBLAS_no{0}))".format(vectorization.defined_macro))
    elif i < len(all_vectorizations) - 1:
      code_block.write("#elif (defined({0}) && !defined(reproBLAS_no{0}))".format(vectorization.defined_macro))
    elif len(all_vectorizations) > 1:
      code_block.write("#else")
    code_block.indent()
    f(vectorization, code_block.sub_block())
    code_block.dedent()
  if len(all_vectorizations) > 1:
    code_block.write("#endif")

def conditionally_include_vectorizations(code_block):
  def include_vectorization(vectorization, code_block):
    if vectorization.header:
      code_block.write("#include <{}>".format(vectorization.header))
  iterate_all_vectorizations(include_vectorization, code_block)
