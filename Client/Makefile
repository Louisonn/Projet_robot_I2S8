#
# Hello Robot (MRPiZ C) - Makefile principal.
# 
# @author Matthias Brun (matthias.brun@eseo.fr)
#

# Répertoire d'installation de la librairie MRPiZ
# pour la compilation à destination de la cible MRPiZ (à adapter) :
LIB_MRPIZ = $(realpath /home/louison/Documents/librairies/C/x86_64/lib_mrpiz-v0.3/)
# exemple : LIB_MRPIZ = $(realpath ../lib_mrpiz-v0.3/)

# Répertoire d'installation des outils de développement croisé (à adapter) :
# CROSS_TOOLS = /<path>/<to>/<cross-compilation>/<tools>/

# Compilateur croisé à utiliser (à adapter)
# CROSS_GCC = $(CROSS_TOOLS)/<relative>/<path>/<to>/<cross>/<gcc>

# Sysroot (à adapter)
# CROSS_SYSROOT = /<path>/<to>/<cross-compilation>/<rootfs>/
# exemple : CROSS_SYSROOT = $(CROSS_SYSROOT)/rootfs_zero_w/


# Adresse du robot MRPiZ (à adapter) :
# MRPIZ_ADDRESS = 192.168.168.106

# Login/mot de passe de connexion au MRPiZ (à adapter) :
MRPIZ_LOGIN = pi
MRPIZ_PASSWORD = raspberry

# Terminal à utiliser pour la connexion ssh (nécessite sshpass et ssh)
TERM = gnome-terminal # gnome-terminal # xterm
TERMOPTIONS = -- # -- (pour gnome-terminal) # -e (pour xterm)

#
# Organisation des sources.
#

export SRCDIR = src
export BINDIR = bin

SUBDIRS = $(SRCDIR)

#
# Définitions des outils.
#

# Compilateur.

ifeq ($(TARGET), mrpiz)
# pour la cible MRPiZ
export CC = $(CROSS_GCC)
export CCFLAGS += -DMRPIZ
export CCFLAGS += --sysroot=$(CROSS_SYSROOT)
export CCFLAGS += -D_POSIX_C_SOURCE=199309L # nécessaire pour timer_t, etc. 
export CCFLAGS += -I$(LIB_MRPIZ)/include/mrpiz/
export LDFLAGS  = -L$(LIB_MRPIZ)/lib/ -lmrpiz -lm

else
# pour le pc de développement (avec simulateur Intox).
export CC = gcc
export CCFLAGS  = -DINTOX -DINTOX_ADDRESS=127.0.0.1 -DINTOX_PORT=12345
export CCFLAGS += -I$(LIB_MRPIZ)/include/mrpiz/
export LDFLAGS  = -L$(LIB_MRPIZ)/lib/ -lintoxmrpiz -lintox

endif

# dans tous les cas.

#sans debuggage : 
export CCFLAGS += -O3 -DNDEBUG 
# avec debuggage : 
# export CCFLAGS += -Og -g -DDEBUG 

export CCFLAGS += -MMD -MP # gestion automatique des dépendances
export CCFLAGS += -D_BSD_SOURCE -D_XOPEN_SOURCE_EXTENDED -D_XOPEN_SOURCE -D_DEFAULT_SOURCE -D_GNU_SOURCE
export CCFLAGS += -std=c99 -Wall -pedantic
export LDFLAGS += -lm -lrt -lpthread

#
# Définitions des binaires à générer.
#
ifeq ($(TARGET), mrpiz)
# pour la cible MRPiZ.
export PROG_NAME = pg_mrpiz
else 
# pour le pc de développement.
export PROG_NAME = pg_mrpiz_intox
endif

export PROG = $(BINDIR)/$(PROG_NAME)

#
# Règles du Makefile.
#

.PHONY: all clean $(SUBDIRS)

# Compilation.
all: $(SUBDIRS)

# Nettoyage.
clean: $(SUBDIRS)
	@rm -f $(PROG) core* $(BINDIR)/core*

# Sous-répertoires.
$(SUBDIRS):
	$(MAKE) $(MAKECMDGOALS) -C $@


#
# Téléchargement sur la cible MRPiZ
#

.PHONY: upload launch term

upload:
ifeq ($(TARGET), mrpiz)
	@sshpass -p '$(MRPIZ_PASSWORD)' scp $(PROG) $(MRPIZ_LOGIN)@$(MRPIZ_ADDRESS):$(PROG_NAME)
endif

# Lancement sur la cible.
launch: upload
ifeq ($(TARGET), mrpiz)
	@$(TERM) $(TERMOPTIONS) sshpass -p '$(MRPIZ_PASSWORD)' ssh -t $(MRPIZ_LOGIN)@$(MRPIZ_ADDRESS) ./$(PROG_NAME)
endif

# Ouvre un terminal sur la cible.
term:
ifeq ($(TARGET), mrpiz)
	@$(TERM) $(TERMOPTIONS) sshpass -p '$(MRPIZ_PASSWORD)' ssh -t $(MRPIZ_LOGIN)@$(MRPIZ_ADDRESS)
endif


#
# Arrêt du programme par ctrl+c
# (utile pour utilisation dans certaines IDE)
#

.PHONY: kill

kill:
	killall -s INT "$(PROG_NAME)"
