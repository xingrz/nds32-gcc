;; Machine description for RISC-V Bit Manipulation operations.
;; Copyright (C) 2021 Free Software Foundation, Inc.

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

(define_code_iterator bitmanip_bitwise [and ior])

(define_code_iterator bitmanip_uminmax [umin umax])

(define_code_iterator bitmanip_sminmax [smin smax])

(define_code_iterator clz_ctz_pcnt [clz ctz popcount])

(define_code_iterator clz_ctz [clz ctz])

(define_code_iterator ctz_pcnt [ctz popcount])

(define_code_attr bitmanip_optab [(smin "smin")
				  (smax "smax")
				  (umin "umin")
				  (umax "umax")
				  (clz "clz")
				  (ctz "ctz")
				  (popcount "popcount")])


(define_code_attr bitmanip_insn [(smin "min")
				 (smax "max")
				 (umin "minu")
				 (umax "maxu")
				 (clz "clz")
				 (ctz "ctz")
				 (popcount "cpop")])

(define_code_attr dsp_insn [(smin "minw")
			    (smax "maxw")])

(define_mode_attr shiftm1 [(SI "const31_operand") (DI "const63_operand")])

;; ZBA extension.

(define_insn "*zero_extendsidi2_bitmanip"
  [(set (match_operand:DI 0 "register_operand" "=r,r")
	(zero_extend:DI (match_operand:SI 1 "nonimmediate_operand" "r,m")))]
  "TARGET_64BIT && TARGET_ZBA"
  "@
   zext.w\t%0,%1
   lwu\t%0,%1"
  [(set_attr "type" "bitmanip,load")
   (set_attr "mode" "DI")])

(define_insn "*shNadd"
  [(set (match_operand:X 0 "register_operand" "=r")
	(plus:X (ashift:X (match_operand:X 1 "register_operand" "r")
			  (match_operand:QI 2 "immediate_operand" "I"))
		(match_operand:X 3 "register_operand" "r")))]
  "TARGET_ZBA
   && (INTVAL (operands[2]) >= 1) && (INTVAL (operands[2]) <= 3)"
  "sh%2add\t%0,%1,%3"
  [(set_attr "type" "bitmanip")
   (set_attr "mode" "<X:MODE>")])

(define_insn "*shNadduw"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(plus:DI
	  (and:DI (ashift:DI (match_operand:DI 1 "register_operand" "r")
			     (match_operand:QI 2 "immediate_operand" "I"))
		 (match_operand 3 "immediate_operand" ""))
	  (match_operand:DI 4 "register_operand" "r")))]
  "TARGET_64BIT && TARGET_ZBA
   && (INTVAL (operands[2]) >= 1) && (INTVAL (operands[2]) <= 3)
   && (INTVAL (operands[3]) >> INTVAL (operands[2])) == 0xffffffff"
  "sh%2add.uw\t%0,%1,%4"
  [(set_attr "type" "bitmanip")
   (set_attr "mode" "DI")])

(define_insn "*add.uw"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(plus:DI (zero_extend:DI
		   (match_operand:SI 1 "register_operand" "r"))
		 (match_operand:DI 2 "register_operand" "r")))]
  "TARGET_64BIT && TARGET_ZBA"
  "add.uw\t%0,%1,%2"
  [(set_attr "type" "bitmanip")
   (set_attr "mode" "DI")])

(define_insn "*slliuw"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(and:DI (ashift:DI (match_operand:DI 1 "register_operand" "r")
			   (match_operand:QI 2 "immediate_operand" "I"))
		(match_operand 3 "immediate_operand" "")))]
  "TARGET_64BIT && TARGET_ZBA
   && (INTVAL (operands[3]) >> INTVAL (operands[2])) == 0xffffffff"
  "slli.uw\t%0,%1,%2"
  [(set_attr "type" "bitmanip")
   (set_attr "mode" "DI")])

;; ZBB extension.

(define_insn "*<optab>_not<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (bitmanip_bitwise:X (not:X (match_operand:X 1 "register_operand" "r"))
                            (match_operand:X 2 "register_operand" "r")))]
  "TARGET_ZBB"
  "<insn>n\t%0,%2,%1"
  [(set_attr "type" "bitmanip")
   (set_attr "mode" "<X:MODE>")])

(define_insn "*xor_not<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (not:X (xor:X (match_operand:X 1 "register_operand" "r")
                      (match_operand:X 2 "register_operand" "r"))))]
  "TARGET_ZBB"
  "xnor\t%0,%1,%2"
  [(set_attr "type" "bitmanip")
   (set_attr "mode" "<X:MODE>")])

(define_insn "<bitmanip_optab>si2"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (ctz_pcnt:SI (match_operand:SI 1 "register_operand" "r")))]
  "TARGET_ZBB"
  { return TARGET_64BIT ? "<bitmanip_insn>w\t%0,%1" : "<bitmanip_insn>\t%0,%1"; }
  [(set_attr "type" "bitmanip")
   (set_attr "mode" "SI")])

(define_insn "clzsi2"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (clz:SI (match_operand:SI 1 "register_operand" "r")))]
  "TARGET_ZBB || (TARGET_DSP && !TARGET_64BIT)"
  {
    if (TARGET_ZBB)
      return TARGET_64BIT ? "clzw\t%0,%1" : "clz\t%0,%1";
    else
      return "clz32\t%0, %1";
  }
  [(set_attr "type" "bitmanip")
   (set_attr "mode" "SI")])

(define_insn "*<bitmanip_optab>disi2"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (sign_extend:DI
          (clz_ctz_pcnt:SI (match_operand:SI 1 "register_operand" "r"))))]
  "TARGET_64BIT && TARGET_ZBB"
  "<bitmanip_insn>w\t%0,%1"
  [(set_attr "type" "bitmanip")
   (set_attr "mode" "SI")])

(define_insn "<bitmanip_optab>di2"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (clz_ctz:DI (match_operand:DI 1 "register_operand" "r")))]
  "TARGET_64BIT && TARGET_ZBB"
  "<bitmanip_insn>\t%0,%1"
  [(set_attr "type" "bitmanip")
   (set_attr "mode" "DI")])

(define_insn_and_split "popcountdi2"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (popcount:DI (match_operand:DI 1 "register_operand" "r")))]
  "TARGET_ZBB"
  "cpop\t%0,%1"
  "TARGET_ZBB && !TARGET_64BIT && !reload_completed"
  [(const_int 1)]
  {
    rtx high = gen_reg_rtx (SImode);
    rtx low = gen_reg_rtx (SImode);
    rtx op0 = gen_reg_rtx (SImode);
    rtx op1 = gen_reg_rtx (SImode);
    high = gen_highpart (SImode, operands[1]);
    low = gen_lowpart (SImode, operands[1]);
    emit_insn (gen_popcountsi2 (op0, low));
    emit_insn (gen_popcountsi2 (op1, high));
    emit_insn (gen_addsi3 (gen_lowpart (SImode, operands[0]), op0, op1));
    DONE;
  }
  [(set_attr "type" "bitmanip")
   (set_attr "mode" "DI")])

(define_insn "*zero_extendhi<GPR:mode>2_bitmanip"
  [(set (match_operand:GPR 0 "register_operand" "=r,r")
        (zero_extend:GPR (match_operand:HI 1 "nonimmediate_operand" "r,m")))]
  "TARGET_ZBB"
  "@
   zext.h\t%0,%1
   lhu\t%0,%1"
  [(set_attr "type" "bitmanip,load")
   (set_attr "mode" "<GPR:MODE>")])

(define_insn "*extend<SHORT:mode><SUPERQI:mode>2_zbb"
  [(set (match_operand:SUPERQI   0 "register_operand"     "=r,r")
	(sign_extend:SUPERQI
	    (match_operand:SHORT 1 "nonimmediate_operand" " r,m")))]
  "TARGET_ZBB"
  "@
   sext.<SHORT:size>\t%0,%1
   l<SHORT:size>\t%0,%1"
  [(set_attr "type" "bitmanip,load")
   (set_attr "mode" "<SUPERQI:MODE>")])

(define_insn "*zero_extendhi<GPR:mode>2_zbb"
  [(set (match_operand:GPR    0 "register_operand"     "=r,r")
	(zero_extend:GPR
	    (match_operand:HI 1 "nonimmediate_operand" " r,m")))]
  "TARGET_ZBB"
  "@
   zext.h\t%0,%1
   lhu\t%0,%1"
  [(set_attr "type" "bitmanip,load")
   (set_attr "mode" "HI")])

(define_insn_and_split "*dsp_si_pair"
  [(set (match_operand:DI   0 "register_operand" "=r")
	(pair:DI
	  (match_operand:SI 1 "register_operand" "r")))]
  "TARGET_DSP && !TARGET_64BIT && !reload_completed"
  "#"
  "&& true"
  [(const_int 1)]
{
  rtx low = riscv_di_low_part_subreg (operands[0]);
  rtx high = riscv_di_high_part_subreg (operands[0]);
  emit_move_insn (low, operands[1]);
  emit_move_insn (high, operands[1]);
  DONE;
})

(define_expand "rotrsi3"
  [(set (match_operand:SI 0 "register_operand")
	(rotatert:SI (match_operand:SI 1 "register_operand")
		     (match_operand:QI 2 "rimm5u_operand")))]
  "TARGET_ZBB || TARGET_DSP"
  {
    if (!TARGET_ZBB && TARGET_DSP)
      {
	if (TARGET_64BIT)
	  {
	    rtx tmp = gen_reg_rtx (DImode);
	    rtx output = gen_reg_rtx (DImode);
	    emit_insn (gen_pkbbdi_3 (tmp, operands[1], operands[1]));
	    if (imm5u_operand (operands[2], QImode))
	      emit_insn (gen_wext64_i (output, tmp, operands[2]));
	    else
	      emit_insn (gen_wext64_r (output, tmp, operands[2]));
	    emit_move_insn (operands[0], lowpart_subreg (SImode, output, DImode));
	    DONE;
	  }
	else
	  {
	    if (!imm5u_operand (operands[2], QImode))
	      FAIL;
	    rtx tmp = gen_reg_rtx (DImode);
	    rtx pair = gen_rtx_PAIR (DImode, operands[1]);
	    emit_move_insn (tmp, pair);
	    emit_insn (gen_wext (operands[0], tmp, operands[2]));
	    DONE;
	  }
      }
  }
  [(set_attr "type" "bitmanip")])

(define_insn "*rotrsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(rotatert:SI (match_operand:SI 1 "register_operand" "r")
		     (match_operand:QI 2 "arith_operand" "rI")))]
  "TARGET_ZBB"
  { return TARGET_64BIT ? "ror%i2w\t%0,%1,%2" : "ror%i2\t%0,%1,%2"; }
  [(set_attr "type" "bitmanip")])

(define_insn "rotrdi3"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(rotatert:DI (match_operand:DI 1 "register_operand" "r")
		     (match_operand:QI 2 "arith_operand" "rI")))]
  "TARGET_64BIT && TARGET_ZBB"
  "ror%i2\t%0,%1,%2"
  [(set_attr "type" "bitmanip")])

(define_insn "rotrsi3_sext"
  [(set (match_operand:DI 0 "register_operand" "=r, r")
	(sign_extend:DI (rotatert:SI (match_operand:SI 1 "register_operand" "r, r")
				     (match_operand:QI 2 "rimm5u_operand"   "r, u05"))))]
  "TARGET_64BIT && TARGET_ZBB"
  "@
   rorw\t%0,%1,%2
   roriw\t%0,%1,%2"
  [(set_attr "type" "bitmanip")])

(define_insn "*rotlsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(rotate:SI (match_operand:SI 1 "register_operand" "r")
		   (match_operand:QI 2 "register_operand" "r")))]
  "TARGET_ZBB"
  { return TARGET_64BIT ? "rolw\t%0,%1,%2" : "rol\t%0,%1,%2"; }
  [(set_attr "type" "bitmanip")])

(define_expand "rotlsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(rotate:SI (match_operand:SI 1 "register_operand")
		   (match_operand:QI 2 "rimm5u_operand")))]
  "TARGET_ZBB || TARGET_DSP"
  {
    if (imm5u_operand (operands[2], QImode))
    {
      if (!TARGET_ZBB && TARGET_DSP)
      {
	if (TARGET_64BIT)
	{
	  rtx tmp = gen_reg_rtx (DImode);
	  rtx output = gen_reg_rtx (DImode);
	  emit_insn (gen_pkbbdi_3 (tmp, operands[1], operands[1]));
	  emit_insn (gen_wext64_i (output, tmp, GEN_INT (32 - INTVAL (operands[2]))));
	  emit_move_insn (operands[0], lowpart_subreg (SImode, output, DImode));
	  DONE;
	}
	else
	{
	  HOST_WIDE_INT shiftamount = INTVAL (operands[2]);
	  rtx tmp = gen_reg_rtx (DImode);
	  rtx pair = gen_rtx_PAIR (DImode, operands[1]);
	  emit_move_insn (tmp, pair);
	  emit_insn (gen_wext (operands[0], tmp, GEN_INT (32 - shiftamount)));
	  DONE;
	}
      }
      else
	FAIL;
    }
    else if (!TARGET_ZBB)
      FAIL;
  }
  [(set_attr "type" "bitmanip")])

(define_insn "rotldi3"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(rotate:DI (match_operand:DI 1 "register_operand" "r")
		   (match_operand:QI 2 "register_operand" "r")))]
  "TARGET_64BIT && TARGET_ZBB"
  "rol\t%0,%1,%2"
  [(set_attr "type" "bitmanip")])

(define_insn "rotlsi3_sext"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(sign_extend:DI (rotate:SI (match_operand:SI 1 "register_operand" "r")
				   (match_operand:QI 2 "register_operand" "r"))))]
  "TARGET_64BIT && TARGET_ZBB"
  "rolw\t%0,%1,%2"
  [(set_attr "type" "bitmanip")])

(define_insn "bswapdi2"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (bswap:DI (match_operand:DI 1 "register_operand" "r")))]
  "TARGET_ZBB && TARGET_64BIT"
  "rev8\t%0,%1"
  [(set_attr "type" "bitmanip")
   (set_attr "mode" "DI")])

(define_insn "*bswapsi2"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (bswap:SI (match_operand:SI 1 "register_operand" "r")))]
  "TARGET_ZBB && !TARGET_64BIT"
  "rev8\t%0,%1"
  [(set_attr "type" "bitmanip")
   (set_attr "mode" "SI")])

(define_expand "bswapsi2"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (bswap:SI (match_operand:SI 1 "register_operand" "r")))]
  "TARGET_ZBB || TARGET_DSP"
  {
    if (TARGET_DSP)
      {
	rtx tmp = gen_reg_rtx (SImode);
	emit_insn (gen_bswap8si2 (tmp, operands[1]));
	emit_insn (gen_bswap16si2 (operands[0], tmp));
        DONE;
      }
    if (TARGET_ZBB && TARGET_64BIT)
      {
	rtx tmp = gen_reg_rtx (DImode);
	rtx input = gen_reg_rtx (DImode);
	rtx output = gen_reg_rtx (DImode);
	emit_move_insn (riscv_di_low_part_subreg (input), operands[1]);
	emit_insn (gen_bswapdi2 (tmp, input));
	emit_insn (gen_lshrdi3 (output, tmp, GEN_INT(32)));
	emit_move_insn (operands[0], riscv_di_low_part_subreg (output));
	DONE;
      }
  }
  [(set_attr "type" "bitmanip")])


(define_insn "<bitmanip_optab><mode>3"
  [(set (match_operand:X 0 "register_operand" "=r")
        (bitmanip_uminmax:X (match_operand:X 1 "register_operand" "r")
			    (match_operand:X 2 "register_operand" "r")))]
  "TARGET_ZBB"
  "<bitmanip_insn>\t%0,%1,%2"
  [(set_attr "type" "bitmanip")])

;; DSP
(define_insn "<bitmanip_optab>di3"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (bitmanip_sminmax:DI (match_operand:DI 1 "register_operand" "r")
			     (match_operand:DI 2 "register_operand" "r")))]
  "TARGET_ZBB && TARGET_64BIT"
  "<bitmanip_insn>\t%0,%1,%2"
  [(set_attr "type" "bitmanip")
   (set_attr "mode" "DI")])

(define_insn "<bitmanip_optab>si3"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (bitmanip_sminmax:SI (match_operand:SI 1 "register_operand" "r")
			     (match_operand:SI 2 "register_operand" "r")))]
  "(TARGET_ZBB && !TARGET_64BIT) || TARGET_DSP"
  { return TARGET_DSP ? "<dsp_insn>\t%0,%1,%2" : "<bitmanip_insn>\t%0,%1,%2"; }
  [(set_attr "type" "bitmanip")
   (set_attr "mode" "SI")])

;; ZBS extension.

(define_insn "*bset<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
	(ior:X (ashift:X (const_int 1)
			 (match_operand:QI 2 "register_operand" "r"))
	       (match_operand:X 1 "register_operand" "r")))]
  "TARGET_ZBS"
  "bset\t%0,%1,%2"
  [(set_attr "type" "bitmanip")])

(define_insn "*bset<mode>_mask"
  [(set (match_operand:X 0 "register_operand" "=r")
	(ior:X (ashift:X (const_int 1)
			 (subreg:QI
			  (and:X (match_operand:X 2 "register_operand" "r")
				 (match_operand 3 "<X:shiftm1>" "i")) 0))
	       (match_operand:X 1 "register_operand" "r")))]
  "TARGET_ZBS"
  "bset\t%0,%1,%2"
  [(set_attr "type" "bitmanip")])

(define_insn "*bset<mode>_1"
  [(set (match_operand:X 0 "register_operand" "=r")
	(ashift:X (const_int 1)
		  (match_operand:QI 1 "register_operand" "r")))]
  "TARGET_ZBS"
  "bset\t%0,x0,%1"
  [(set_attr "type" "bitmanip")])

(define_insn "*bset<mode>_1_mask"
  [(set (match_operand:X 0 "register_operand" "=r")
	(ashift:X (const_int 1)
		  (subreg:QI
		   (and:X (match_operand:X 1 "register_operand" "r")
			  (match_operand 2 "<X:shiftm1>" "i")) 0)))]
  "TARGET_ZBS"
  "bset\t%0,x0,%1"
  [(set_attr "type" "bitmanip")])

(define_insn "*bseti<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
	(ior:X (match_operand:X 1 "register_operand" "r")
	       (match_operand 2 "single_bit_mask_operand" "i")))]
  "TARGET_ZBS"
  "bseti\t%0,%1,%S2"
  [(set_attr "type" "bitmanip")])

(define_insn "*bclr<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
	(and:X (rotate:X (const_int -2)
			 (match_operand:QI 2 "register_operand" "r"))
	       (match_operand:X 1 "register_operand" "r")))]
  "TARGET_ZBS"
  "bclr\t%0,%1,%2"
  [(set_attr "type" "bitmanip")])

(define_insn "*bclri<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
	(and:X (match_operand:X 1 "register_operand" "r")
	       (match_operand 2 "not_single_bit_mask_operand" "i")))]
  "TARGET_ZBS"
  "bclri\t%0,%1,%T2"
  [(set_attr "type" "bitmanip")])

(define_insn "*binv<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
	(xor:X (ashift:X (const_int 1)
			 (match_operand:QI 2 "register_operand" "r"))
	       (match_operand:X 1 "register_operand" "r")))]
  "TARGET_ZBS"
  "binv\t%0,%1,%2"
  [(set_attr "type" "bitmanip")])

(define_insn "*binvi<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
	(xor:X (match_operand:X 1 "register_operand" "r")
	       (match_operand 2 "single_bit_mask_operand" "i")))]
  "TARGET_ZBS"
  "binvi\t%0,%1,%S2"
  [(set_attr "type" "bitmanip")])

(define_insn "*bext<mode>"
  [(set (match_operand:X 0 "register_operand" "=r")
	(zero_extract:X (match_operand:X 1 "register_operand" "r")
			(const_int 1)
			(zero_extend:X
			 (match_operand:QI 2 "register_operand" "r"))))]
  "TARGET_ZBS"
  "bext\t%0,%1,%2"
  [(set_attr "type" "bitmanip")])

(define_insn "*bexti"
  [(set (match_operand:X 0 "register_operand" "=r")
	(zero_extract:X (match_operand:X 1 "register_operand" "r")
			(const_int 1)
			(match_operand 2 "immediate_operand" "i")))]
  "TARGET_ZBS"
  "bexti\t%0,%1,%2"
  [(set_attr "type" "bitmanip")])
