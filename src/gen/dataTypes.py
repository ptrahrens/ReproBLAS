################################################################################
# dataTypes.py                                                                 #
#                                                                              #
#     A set of classes used to represent the four main data types supported by #
# ReproBLAS.                                                                   #
#                                                                              #
#                                                            Willow Ahrens 2014 #
################################################################################

from utils import *

class DataType(object):
  name_char = ''
  name = "" #name of type
  int_float_name = ""
  int_char = ""
  float_char = ""
  base_type = None #base type (base type of double complex is double)
  byte_size = 0
  bit_size = 0
  base_size = 0
  is_complex = False

  def index(self, offset, inc, i, base_index = True):
    if(self.is_complex and base_index):
      return mix("+", mix("*", self.base_size, inc, mix("+", offset, mix("//", i, 2))), mix("%", i, 2))
    else:
      return mix("*", inc, mix("+", offset, i))

  ##
  #  @brief the standard loop increment string
  #
  #  @param src_ptrs names of pointers to source variables to be incremented(must be of type base_type)
  #  @param src_incs names of increment variables
  #  @param i how much to increment
  #  @return a string that performs the increment by i of all source pointers
  #
  def data_increment(self, src_ptrs, src_incs, i):
    return ", ".join(["{0} += {1}".format(src_ptr, mix("*", src_inc, self.base_size, i)) for (src_ptr, src_inc) in zip(src_ptrs, src_incs)])

class Double(DataType):
  name_char = 'd'
  name = "double"
  int_float_name = "long_double"
  int_char = "l"
  float_char = "d"
  byte_size = 8
  bit_size = 64
  base_size = 1

  def __init__(self, code_block):
    self.code_block = code_block
Double.base_type = Double

class DoubleComplex(DataType):
  name_char = 'z'
  name = "double complex"
  byte_size = Double.byte_size * 2
  bit_size = Double.bit_size * 2
  base_size = 2
  is_complex = True
  base_type = Double

  def __init__(self, code_block):
    self.code_block = code_block

class Float(DataType):
  name_char = 's'
  name = "float"
  int_float_name = "int_float"
  int_char = "i"
  float_char = "f"
  byte_size = 4
  bit_size = 32
  base_size = 1

  def __init__(self, code_block):
    self.code_block = code_block
Float.base_type = Float


class FloatComplex(DataType):
  name_char = 'c'
  name = "float complex"
  byte_size = Float.byte_size * 2
  bit_size = Float.bit_size * 2
  base_size = 2
  is_complex = True
  base_type = Float

  def __init__(self, code_block):
    self.code_block = code_block
