# Makefile de ejemplo para C++
#
# Creado: jue abr 15 15:34:19 ART 2004
#
# Copyleft 2004 - Leandro Lucarella, Bajo licencia GPL [http://www.gnu.org/]
#

# CONFIGURACION
################

# Nombre del ejecutable.
target_ppal = agua
target_procesador = agua_procesador

# Si usa funciones de math.h, descomentá (quitale el '#' a) la siguiente línea.
math = si

# Si usa pthreads, descomentá (quitale el '#' a) la siguiente línea.
pthreads = si

# Si usa gthreads, descomentá (quitale el '#' a) la siguiente línea.
#gthreads = si

# Si es un programa GTK+, descomentá (quitale el '#' a) la siguiente línea.
gtk = si
xml = si
# Si es un programa gtkmm, descomentá (quitale el '#' a) la siguiente línea.
#gtkmm = si

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

# Agrego flags y libs de GTK+ de ser necesario.
ifdef gtkmm
CFLAGS += $(shell pkg-config --cflags gtkmm-2.4)
LDFLAGS += $(shell pkg-config --libs gtkmm-2.4)
endif

# Agrego flags y libs de gthreads de ser necesario.
ifdef gthreads
CFLAGS += $(shell pkg-config --cflags gthread-2.0)
LDFLAGS += $(shell pkg-config --libs gthread-2.0)
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
fuentes ?= $(wildcard *.$(extension))
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


clean:
	$(RM) $(target_ppal) $(target_procesador)

