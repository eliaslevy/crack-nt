# Note that this makefile probably depends on the standard GNU make-rules
# (Oh, and I'm not really sure why I'm building the MD4 binary...)

# CONFIGURE-ME : compiler and linker options
CC	=gcc
CCOPTS	=-Wall -pedantic -O6 -fomit-frame-pointer -funroll-loops
DESOPS  =-DDES_UNROLL -DDES_RISC1 -DDES_PTR
LDOPTS	=-s
# CONFIGURE-ME : libdes directory and library
DESDIR	=../libdes
LIBDES=../libdes/libdes.a

# You shouldn't need to touch these
CFLAGS	=$(CCOPTS) $(DESOPTS) -I$(DESDIR) -DMD=4
LDFLAGS	=$(LDOPTS)
RM_F	=rm -f
AR_CR	=ar -cr
# Comment out if you don't have/need this
RANLIB	=ranlib

####                       ####
#                             #
# END OF CONFIGURABLE OPTIONS #
#                             #
####                       ####

LIB	=libcrypt_nt.a
LIBOBJS	=md4.o crypt_nt.o raw_lanman.o
BINS	=md4 hash_nt speed_nt
BINOBJS	=mddriver.o hash_nt.o speed_nt.o lanman2nt.o
OBJS	=$(LIBOBJS) $(BINOBJS)

all:$(LIB) lanman2nt

test:$(BINS)
	# ./md4       does (standard) MD4 hashing
	# ./hash_nt   does both forms of NT hashing
	# ./test_md4  [script] checks (standard) MD4 hashing works
	# ./test_nt   [script] checks both forms of NT hashing work
	# ./speed_nt  calculates the number of NT hashes per second

clean:
	$(RM_F) $(LIB) $(OBJS) $(BINS) a.out core *~ ~* \#*

# Depending on libdes.a here is a kludge to save me having to
# trawl through the Crack makefiles for how to do it properly ;-) 
$(LIB):$(LIBOBJS) $(LIBDES)
	$(AR_CR) $(LIB) $(LIBOBJS)
	if [ ! -z "$(RANLIB)" ] ; then $(RANLIB) $(LIB) ; fi
		
$(LIBDES):
	cd $(DESDIR) && make `basename $(LIBDES)`

md4	:md4.o mddriver.o
hash_nt	:$(LIB) $(LIBDES) hash_nt.o
speed_nt:$(LIB) $(LIBDES) speed_nt.o
lanman2nt:$(LIB) $(LIBDES) lanman2nt.o
