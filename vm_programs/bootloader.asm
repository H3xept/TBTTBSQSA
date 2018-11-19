	HALT
# location 4074
bootloader:
	LOAD $s1 1	
	LOAD $s2 1	
	LOAD $s3 2	
	LOAD $s4 0b110		
	MOVE $t1 $fr
	SHR $t1 $s1	
	AND $t1 $s2	
	SKC $t1		
	JUMP 1
# location 4087
loop:			
	IN $t1		
	STORE $t1 $s1
	ADD $s1 $s2	
	MOVE $t1 $fr
	SHR $t1 $s1	
	AND $t1 $s2	
	SKC $t1		
	JUMP 1		
	JUMP loop	
# end of memory

# 0000 0000 0000 0000
# 0011 0000 0001 1001
# 0000 0000 0000 0001
# 0011 0000 0001 1010
# 0000 0000 0000 0001
# 0011 0000 0001 1011
# 0000 0000 0000 0010
# 0011 0000 0001 1100
# 0000 0000 0000 0110
# 0111 0000 0101 1111
# 1111 0000 0101 1001
# 1011 0000 0101 1010
# 0010 0000 0000 0101
# 0001 0000 0000 0001
# 0101 0000 0000 0101
# 0100 0000 0101 1001
# 1000 0000 1001 1010
# 0111 0000 0101 1111
# 1111 0000 0101 1001
# 1011 0000 0101 1010
# 0010 0000 0000 0101
# 0001 0000 0000 0001
# 0001 1111 1111 0111

# 000030190001301a0001301b0002301c0006705ff059b05a2005100150054059809a705ff059b05a200510011ff7