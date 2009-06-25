# Makefile de ejemplo para C++
#
# Creado: jue abr 15 15:34:19 ART 2004
#
# Ultima modificacion: jue jun 25 11:23:42 ART 2009
#
# Copyleft 2004 - Leandro Lucarella, Bajo licencia GPL [http://www.gnu.org/]
# 

# CONFIGURACION
################

# Nombre del ejecutable.
target_ppal = agua
target_procesador = agua_procesador

ifndef PREFIX
PREFIX=/usr/local
endif

# Si usa funciones de math.h, descomentá (quitale el '#' a) la siguiente línea.
math = si

# Si usa pthreads, descomentá (quitale el '#' a) la siguiente línea.
pthreads = si

# Si es un programa GTK+, descomentá (quitale el '#' a) la siguiente línea.
gtk = si

xml = si

# Descomentar si se quiere ver como se invoca al compilador
#verbose = si

# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C/C++ en modo ansi.
#CFLAGS = -Wall -pedantic -pedantic-errors

CFLAGS += -O3 -DNDEBUG

# Para valgrind o debug
#CFLAGS += -ggdb -DDEBUG -fno-inline

# Opciones del enlazador.
#LDFLAGS =

# Estandar de C a usar
CSTD = c99

# Estandar de C++ a usar
CXXSTD = c++98

# Si se quiere compilar estaticamente, descomentar la siguiente linea
#static = si


# VARIABLES CALCULADAS A PARTIR DE LA CONFIGURACION
####################################################

# Agrego flags y libs de GTK+ de ser necesario.
ifdef gtk
CFLAGS += $(shell pkg-config --cflags gtk+-2.0)
LDFLAGS += $(shell pkg-config --libs gtk+-2.0)
endif

# Agrego flags y libs de libxml de ser necesario.
ifdef xml
CFLAGS += $(shell pkg-config --cflags libxml-2.0)
LDFLAGS += $(shell pkg-config --libs libxml-2.0)
endif

# Linkeo con libm de ser necesario.
ifdef math
LDFLAGS += -lm
endif

# Linkeo con pthreads de ser necesario.
ifdef pthreads
LDFLAGS += -lpthread -pthread
endif

ifdef static
LDFLAGS += -static
endif

# Pongo flags de C para C++ también
CXXFLAGS += $(CFLAGS)

extension=c

# Uso enlazador de c++ si es código no C.
ifeq ($(extension), c)
CFLAGS += -std=$(CSTD)
LD = $(CC)
else
CXXFLAGS += -std=$(CXXSTD)
LD = $(CXX)
endif

# Si no especifica archivos, tomo todos.
fuentes_procesador = agua_procesador.c parser.c rb.c archivo.c particiones.c lista.c nodo.c pila.c debug.c ReplacementSelection.c merge.c matrizBayes.c
fuentes_ppal = agua.c main.c Controlador/controlador.c GUI/*/*.c archivo.c matrizBayes.c lista.c nodo.c
occ := $(CC)
ocxx := $(CXX)
orm := $(RM)
old := $(LD)
ifdef verbose
RM := $(RM) -v
else
CC =  @echo "  CC  $@"; $(occ)
CXX = @echo "  CXX $@"; $(ocxx)
RM =  @echo "  CLEAN"; $(orm)
LD =  @echo "  LD  $@"; $(old)
endif


# REGLAS
#########

.PHONY: all clean

all: procesador principal

procesador: $(fuentes_procesador)
	$(LD) $(LDFLAGS)  $(CFLAGS) $(fuentes_procesador) $(LOADLIBS) $(LDLIBS) -o $(target_procesador)

principal: $(fuentes_ppal)
	$(LD) $(LDFLAGS) $(CFLAGS) $(fuentes_ppal) $(LOADLIBS) $(LDLIBS) -o $(target_ppal)

install: $(all)
	mkdir -p $(PREFIX)/bin/
	mkdir -p $(PREFIX)/bin/GUI/Vista/
	cp GUI/Vista/*.glade $(PREFIX)/bin/GUI/Vista/
	cp GUI/Vista/*.png $(PREFIX)/bin/GUI/Vista/
	cp GUI/Vista/*.gif $(PREFIX)/bin/GUI/Vista/
	cp $(target_procesador) $(PREFIX)/bin/
	cp $(target_ppal) $(PREFIX)/bin/

uninstall:
	@if [ -r $(PREFIX)/bin/$(target_ppal) ]; \
	then \
		rm $(PREFIX)/bin/$(target_ppal); \
	fi
	@if [ -r $(PREFIX)/bin/$(target_procesador) ]; \
	then \
		rm $(PREFIX)/bin/$(target_procesador); \
	fi


clean:
	$(RM) $(target_ppal) $(target_procesador)

