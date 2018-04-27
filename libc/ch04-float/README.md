# float.h

In the case of C, this will actually be bundled in from gcc.
In the gcc source code tree you can see that the constants used to defined the
macros are themselves geenrated from `gcc/c-family/c-cppbuiltin.c:builtin_define_float_constants`.

The function is defined as follows (`gcc-5.4.0`):
```C
/* Define the float.h constants for TYPE using NAME_PREFIX, FP_SUFFIX,          
   and FP_CAST. */                                                              
static void                                                                     
builtin_define_float_constants (const char *name_prefix,                        
                        const char *fp_suffix,                                  
                const char *fp_cast,                                            
                const char *fma_suffix,                                         
                tree type)                                                      
{                                                                               
  /* Used to convert radix-based values to base 10 values in several cases.     
                                                                                
     In the max_exp -> max_10_exp conversion for 128-bit IEEE, we need at       
     least 6 significant digits for correct results.  Using the fraction        
     formed by (log(2)*1e6)/(log(10)*1e6) overflows a 32-bit integer as an      
     intermediate; perhaps someone can find a better approximation, in the      
     mean time, I suspect using doubles won't harm the bootstrap here.  */      
                                                                                
  const double log10_2 = .30102999566398119521;                                 
  double log10_b;                                                               
  const struct real_format *fmt;                                                
  const struct real_format *ldfmt;                                              
                                                                                
  char name[64], buf[128];                                                      
  int dig, min_10_exp, max_10_exp;                                              
  int decimal_dig;                                                              
  int type_decimal_dig;                                                         
                                                                                
  fmt = REAL_MODE_FORMAT (TYPE_MODE (type));                                    
  gcc_assert (fmt->b != 10);                                                    
  ldfmt = REAL_MODE_FORMAT (TYPE_MODE (long_double_type_node));                 
  gcc_assert (ldfmt->b != 10);                                                  
                                                                                
  /* The radix of the exponent representation.  */                              
  if (type == float_type_node)                                                  
    builtin_define_with_int_value ("__FLT_RADIX__", fmt->b);                    
  log10_b = log10_2;                                                            
                                                                                
  /* The number of radix digits, p, in the floating-point significand.  */      
  sprintf (name, "__%s_MANT_DIG__", name_prefix);                               
  builtin_define_with_int_value (name, fmt->p);                                 
                                                                                
  /* The number of decimal digits, q, such that any floating-point number       
     with q decimal digits can be rounded into a floating-point number with     
     p radix b digits and back again without change to the q decimal digits,    
                                                                                
    p log10 b           if b is a power of 10                                   
    floor((p - 1) log10 b)      otherwise                                       
  */
  dig = (fmt->p - 1) * log10_b;                                                 
  sprintf (name, "__%s_DIG__", name_prefix);                                    
  builtin_define_with_int_value (name, dig);                                    
                                                                                
  /* The minimum negative int x such that b**(x-1) is a normalized float.  */   
  sprintf (name, "__%s_MIN_EXP__", name_prefix);                                
  sprintf (buf, "(%d)", fmt->emin);                                             
  builtin_define_with_value (name, buf, 0);                                     
                                                                                
  /* The minimum negative int x such that 10**x is a normalized float,          
                                                                                
      ceil (log10 (b ** (emin - 1)))                                            
    = ceil (log10 (b) * (emin - 1))                                             
                                                                                
     Recall that emin is negative, so the integer truncation calculates         
     the ceiling, not the floor, in this case.  */                              
  min_10_exp = (fmt->emin - 1) * log10_b;                                       
  sprintf (name, "__%s_MIN_10_EXP__", name_prefix);                             
  sprintf (buf, "(%d)", min_10_exp);                                            
  builtin_define_with_value (name, buf, 0);                                     
                                                                                
  /* The maximum int x such that b**(x-1) is a representable float.  */         
  sprintf (name, "__%s_MAX_EXP__", name_prefix);                                
  builtin_define_with_int_value (name, fmt->emax);                              
                                                                                
  /* The maximum int x such that 10**x is in the range of representable         
     finite floating-point numbers,                                             
                                                                                
      floor (log10((1 - b**-p) * b**emax))                                      
    = floor (log10(1 - b**-p) + log10(b**emax))                                 
    = floor (log10(1 - b**-p) + log10(b)*emax)                                  
                                                                                
     The safest thing to do here is to just compute this number.  But since     
     we don't link cc1 with libm, we cannot.  We could implement log10 here     
     a series expansion, but that seems too much effort because:                
                                                                                
     Note that the first term, for all extant p, is a number exceedingly close  
     to zero, but slightly negative.  Note that the second term is an integer   
     scaling an irrational number, and that because of the floor we are only    
     interested in its integral portion.                                        
                                                                                
     In order for the first term to have any effect on the integral portion     
     of the second term, the second term has to be exceedingly close to an      
     integer itself (e.g. 123.000000000001 or something).  Getting a result     
     that close to an integer requires that the irrational multiplicand have    
     a long series of zeros in its expansion, which doesn't occur in the        
     first 20 digits or so of log10(b).                                         
                                                                                
     Hand-waving aside, crunching all of the sets of constants above by hand    
     does not yield a case for which the first term is significant, which       
     in the end is all that matters.  */                                        
  max_10_exp = fmt->emax * log10_b;                                             
  sprintf (name, "__%s_MAX_10_EXP__", name_prefix);                             
  builtin_define_with_int_value (name, max_10_exp); 
                                                                                
  /* The number of decimal digits, n, such that any floating-point number       
     can be rounded to n decimal digits and back again without change to        
     the value.                                                                 
                                                                                
    p * log10(b)            if b is a power of 10                               
    ceil(1 + p * log10(b))      otherwise                                       
                                                                                
     The only macro we care about is this number for the widest supported       
     floating type, but we want this value for rendering constants below.  */   
  {                                                                             
    double d_decimal_dig                                                        
      = 1 + (fmt->p < ldfmt->p ? ldfmt->p : fmt->p) * log10_b;                  
    decimal_dig = d_decimal_dig;                                                
    if (decimal_dig < d_decimal_dig)                                            
      decimal_dig++;                                                            
  }                                                                             
  /* Similar, for this type rather than long double.  */                        
  {                                                                             
    double type_d_decimal_dig = 1 + fmt->p * log10_b;                           
    type_decimal_dig = type_d_decimal_dig;                                      
    if (type_decimal_dig < type_d_decimal_dig)                                  
      type_decimal_dig++;                                                       
  }                                                                             
  if (type == long_double_type_node)                                            
    builtin_define_with_int_value ("__DECIMAL_DIG__", decimal_dig);             
  else                                                                          
    {                                                                           
      sprintf (name, "__%s_DECIMAL_DIG__", name_prefix);                        
      builtin_define_with_int_value (name, type_decimal_dig);                   
    }                                                                           
                                                                                
  /* Since, for the supported formats, B is always a power of 2, we             
     construct the following numbers directly as a hexadecimal                  
     constants.  */                                                             
  get_max_float (fmt, buf, sizeof (buf));                                       
                                                                                
  sprintf (name, "__%s_MAX__", name_prefix);                                    
  builtin_define_with_hex_fp_value (name, type, decimal_dig, buf, fp_suffix, fp_cast);
                                                                                
  /* The minimum normalized positive floating-point number,                     
     b**(emin-1).  */                                                           
  sprintf (name, "__%s_MIN__", name_prefix);                                    
  sprintf (buf, "0x1p%d", fmt->emin - 1);                                       
  builtin_define_with_hex_fp_value (name, type, decimal_dig, buf, fp_suffix, fp_cast);
                                                                                
  /* The difference between 1 and the least value greater than 1 that is        
     representable in the given floating point type, b**(1-p).  */              
  sprintf (name, "__%s_EPSILON__", name_prefix);                                
  if (fmt->pnan < fmt->p)                                                       
    /* This is an IBM extended double format, so 1.0 + any double is            
       representable precisely.  */                                             
      sprintf (buf, "0x1p%d", fmt->emin - fmt->p);                              
    else                                                                        
      sprintf (buf, "0x1p%d", 1 - fmt->p);                                      
  builtin_define_with_hex_fp_value (name, type, decimal_dig, buf, fp_suffix, fp_cast);

  /* For C++ std::numeric_limits<T>::denorm_min and C11 *_TRUE_MIN.             
     The minimum denormalized positive floating-point number, b**(emin-p).      
     The minimum normalized positive floating-point number for formats          
     that don't support denormals.  */                                          
  sprintf (name, "__%s_DENORM_MIN__", name_prefix);                             
  sprintf (buf, "0x1p%d", fmt->emin - (fmt->has_denorm ? fmt->p : 1));          
  builtin_define_with_hex_fp_value (name, type, decimal_dig,                    
                    buf, fp_suffix, fp_cast);                                   
                                                                                
  sprintf (name, "__%s_HAS_DENORM__", name_prefix);                             
  builtin_define_with_value (name, fmt->has_denorm ? "1" : "0", 0);             
                                                                                
  /* For C++ std::numeric_limits<T>::has_infinity.  */                          
  sprintf (name, "__%s_HAS_INFINITY__", name_prefix);                           
  builtin_define_with_int_value (name,                                          
                 MODE_HAS_INFINITIES (TYPE_MODE (type)));                       
  /* For C++ std::numeric_limits<T>::has_quiet_NaN.  We do not have a           
     predicate to distinguish a target that has both quiet and                  
     signalling NaNs from a target that has only quiet NaNs or only             
     signalling NaNs, so we assume that a target that has any kind of           
     NaN has quiet NaNs.  */                                                    
  sprintf (name, "__%s_HAS_QUIET_NAN__", name_prefix);                          
  builtin_define_with_int_value (name, MODE_HAS_NANS (TYPE_MODE (type)));       
                                                                                
  /* Note whether we have fast FMA.  */                                         
  if (mode_has_fma (TYPE_MODE (type)))                                          
    {                                                                           
      sprintf (name, "__FP_FAST_FMA%s", fma_suffix);                            
      builtin_define_with_int_value (name, 1);                                  
    }                                                                           
}
```
