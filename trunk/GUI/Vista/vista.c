#include "vista.h"

void crearVista(Vista* vista, Controlador* controlador) {

   	vista->controlador= controlador;
	
	GError*	error= NULL; 
	vista->builder= gtk_builder_new();
   	if(!gtk_builder_add_from_file(vista->builder, PATH_VISTA, &error))
   		printf("Error: %s\n", error->message);
}

void connect_accept(GtkWidget *widget, Vista* vista) {
	
	procesarPalabras(vista->controlador);
}

void correrVista(Vista* vista) {
	
	GtkWidget  *window;

   	window= GTK_WIDGET(gtk_builder_get_object(vista->builder, "window"));
   	gtk_widget_show_all(window);
   
   	gtk_builder_connect_signals(vista->builder, NULL);
   	
   	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
   	
   	GtkWidget  *buttonAccept;
   	buttonAccept= GTK_WIDGET(gtk_builder_get_object(vista->builder, "button_accept"));
   	
   	g_signal_connect(G_OBJECT(buttonAccept), "clicked", G_CALLBACK(connect_accept), vista);
   	
   	gtk_main();
}

char* obtenerPalabrasVista(Vista* vista) {

	GtkEntry *entryWords;
	entryWords= GTK_ENTRY(gtk_builder_get_object(vista->builder, "entry_words"));
	return (char*)gtk_entry_get_text(entryWords);
}

void destruirVista(Vista* vista) { }
