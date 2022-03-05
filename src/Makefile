#Used LOP profile directions to get a profile of the code.
#Uncomment and run the code long enough to get a good profile and do a normal ingame shutdown.
#Go to the "6dragons/" directory.
#Then do a "prof 6dragons > prof.out"
#PROF    = -p
#Used to get an advanced profile of the code.
#Do same as above one only use "gprof 6dragons > gprof.out"
#Then just look at "prof.out/gprof.out" in an editor to see the profile.
#PROF    = -pg

#Uncomment the line below if you'll be using gdb for debugging information
USE_GDB = gdb
DEBUG_LEVEL = 3

#The program executable's name
PROG_NAME = 6dragons

#Memory Debugger - comment out to turn the debugger off, uncomment to use it.
MEMDEBUG = 1

#Comment this if you don't want math errors
#MATH_LIB = -lm

#Uncomment the line below if compiling on a RedHat (and possibly other Linux) box
#LINUX = -DLINUX

#Uncomment the line below if you are getting a line like:
#interp.c:757: warning: int format, time_t arg (arg 7)
#TIME = -DTIMEFORMAT

#I3 - Comment out to disable I3 support
I3 = 1

#MEMWATCH - Comment out to disable Memwatch check for memory leaks
#MEMWATCH = 1

#Uncomment the line below if you are getting undefined crypt errors
#NEED_CRYPT = -lcrypt

#Uncomment the line below if you are getting undefined references to dlsym, dlopen, and dlclose.
#Comment it out if you get errors about ldl not being found.
#NEED_DL = -ldl

#Bexar added this for Lua support
#LUA = -llua

#Directory locations of important lib files
LIBDIR  = ./lib

#Modify the lines below if you want a performance increase though beware your core
#files may not be as much of a benefit if you do. Suggested OPT_FLAG: -O
OPT_LEVEL = 
OPT_FLAG = -O$(OPT_LEVEL)

#Warning check flags
W_FLAGS = -Wall -Wunused -Werror -Wuinitialized -Wshadow -Wformat-security -Wpointer-arith -Wcast-align -Wredundant-decls -Wconversion $(WSTRINGS) 

#Compile and Link flags
ifdef MEMWATCH
CMP_FLG = -g -O $(WRN_FLG) -DMEMWATCH -DMEMWATCH_STDIO
else
CMP_FLG = -g -O $(WRN_FLG) 
endif
C_FLAGS = $(OPT_FLAG) $(CMP_FLG) $(PROF) $(NOCRYPT) $(SOLARIS_FLAG) $(TIME) $(LINUX)
L_FLAGS = $(PROF) $(SOLARIS_LINK) -lz -lstdc++ $(MATH_LIB) 
#If there are any problems linking with -lz you may need sudo apt-get install zlib1g-dev 

#Listing of all '.c' files DO NOT GET OUT OF ALPHABETICAL ORDER
C_FILES = act_comm.c      act_info.c       act_move.c     act_obj.c    act_wiz.c    alias.c     archery.c   location.c \
          arena.c         automap.c        ban.c          bank.c       boards.c     build.c     buitdy.c    calendar.c  city.c \
          channels.c      clans.c          classes.c      color.c      comm.c       companion.c const.c     copyover.c  coalesce.c \
          combo.c         currency.c      custom.c         crafts.c       db.c         dragon.c    editor.c    events.c  \
          fight.c         filer.c          ftag.c         genesis.c    handler.c    hint.c      hometowns.c  house.c \
          interp.c        landmark.c       magic.c        mccp.c       memory.c     misc.c  \
          monk.c          mssp.c           mud_comm.c     mud_prog.c   multi.c      pets.c      new_auth.c  news.c \
          player.c        polymorph.c      portalstone.c  quests.c     races.c     renumber.c  reset.c run.c \
          save.c          ships.c          shops.c          sha256.c     skills.c     smell.c      special.c   status.c   symref.c \
          teacher.c       track.c          transfer.c       update.c       variables.c  \
          tables.c        weather.c        whereis.c      whois.c      wild_comm.c  wild_mobs.c      

#Listing of all '.h' files necessary to create the program binary

ifdef I3
   C_FILES := i3.c $(C_FILES)
   C_FLAGS := $(C_FLAGS) -DI3 -DI3SMAUG
endif

ifdef MEMWATCH
   C_FILES := memwatch.c $(C_FILES)
endif

O_FILES := $(patsubst %.c,o/%.o,$(C_FILES))

H_FILES = $(wildcard h/*.h)

all:
	@$(MAKE) -s $(PROG_NAME)

$(PROG_NAME): $(O_FILES)
	@rm -f $(PROG_NAME)
	@$(CC) $(L_FLAGS) -o $(PROG_NAME) $(O_FILES) $(L_FLAGS)
	@chmod 700 $(C_FILES)
	@chmod 700 $(H_FILES)
	@chmod g+w $(PROG_NAME)
	@chmod a+x $(PROG_NAME)
	@chmod g+w $(O_FILES)
	mv $(PROG_NAME) ../$(PROG_NAME)
	@echo "Done Compiling $(PROG_NAME)"

symref.c: h/mud.h symref_template.c mktables h/landmark.h 
	./mktables symref.c symref_template.c -h h/mud.h -h h/landmark.h 

o/%.o: %.c $(H_FILES)
	echo "  Compiling $@";
	$(CC) -x c++  -c $(C_FLAGS) $< -o $@

.c.o: mud.h
	$(CC) -c $(C_FLAGS) $<

clean:
	rm -f o/*.o ../$(PROG_NAME) *~ h/*~ symref.c
	make all

purge:
	rm -f o/*.o ../$(PROG_NAME) *~ h/*~ symref.c

indent:
	indent -ts2 -nut -nsaf -nsai -nsaw -npcs -npsl -ncs -nbc -bls -nprs -bap -cbi0 -cli2 -bli0 -l200 -lp -i2 -cdb -c1 -cd1 -sc -pi0 $(C_FILES)
	indent -ts2 -nut -nsaf -nsai -nsaw -npcs -npsl -ncs -nbc -bls -nprs -bap -cbi0 -cli2 -bli0 -l200 -lp -i2 -cdb -c1 -cd1 -sc -pi0 $(H_FILES)

indentclean:
	rm *.c~ h/*.h~
