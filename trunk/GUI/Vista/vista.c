#include "vista.h"

void crearVista(Vista* vista, Controlador* controlador) {

   	vista->controlador= controlador;
	 
	GError*	error= NULL; 
	vista->builder= gtk_builder_new();
   	if(!gtk_builder_add_from_file(vista->builder, PATH_VISTA, &error))
   		printf("Error: %s\n", error->message);
}

void mostrarDistraccion(Vista* vista) {
	
	vista->dialog= GTK_WIDGET(gtk_builder_get_object(vista->builder, "dialog_fun"));
 	GtkWidget* area= gtk_dialog_get_content_area(GTK_DIALOG(vista->dialog));
 	gtk_widget_show_all(area);
 	GtkWidget* box= GTK_WIDGET(gtk_builder_get_object(vista->builder, "dialog-vbox3"));
 	gtk_widget_show_all(box);
	GtkWidget* image= GTK_WIDGET(gtk_builder_get_object(vista->builder, "image_fun"));
 	gtk_widget_show_all(image);
 	GtkWidget* label= GTK_WIDGET(gtk_builder_get_object(vista->builder, "label_fun"));
 	gtk_widget_show_all(label);
  	gtk_widget_show_all(vista->dialog);
  	
  	GtkWidget* box2= GTK_WIDGET(gtk_builder_get_object(vista->builder, "dialog-vbox3"));
  	gtk_widget_show_all(box2);
 	GtkWidget* area2= gtk_dialog_get_action_area(GTK_DIALOG(vista->dialog));
 	gtk_widget_show_all(area2);

}

void cerrarDistraccion(Vista* vista) {

	gtk_widget_hide_all(vista->dialog);	
}

void mostrarNoEncontrado(Vista* vista) {

	GtkWidget *dialog;
   	dialog= GTK_WIDGET(gtk_builder_get_object(vista->builder, "dialog_not_found"));
  	gtk_dialog_run(GTK_DIALOG(dialog));
  	gtk_widget_hide(dialog);
}

void connect_accept(GtkWidget *widget, Vista* vista) {
	
	procesarPalabras(vista->controlador);
}

void connect_clear(GtkWidget *widget, Vista* vista) {
	
	limpiarEntrys(vista);
}

void correrVista(Vista* vista) {
	
	GtkWidget  *window;

   	window= GTK_WIDGET(gtk_builder_get_object(vista->builder, "window"));
   	
   	gtk_builder_connect_signals(vista->builder, NULL);
   	
   	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
   	
   	GtkWidget  *buttonAccept;
   	buttonAccept= GTK_WIDGET(gtk_builder_get_object(vista->builder, "button_accept"));
   	g_signal_connect(G_OBJECT(buttonAccept), "clicked", G_CALLBACK(connect_accept), vista);

	GtkWidget  *buttonClear;
   	buttonClear= GTK_WIDGET(gtk_builder_get_object(vista->builder, "button_clear"));
   	g_signal_connect(G_OBJECT(buttonClear), "clicked", G_CALLBACK(connect_clear), vista);

   	gtk_widget_show_all(window);
   	gtk_main();
}

char* obtenerPalabrasVista(Vista* vista) {

	GtkEntry *entryWords;
	entryWords= GTK_ENTRY(gtk_builder_get_object(vista->builder, "entry_words"));
	return (char*)gtk_entry_get_text(entryWords);
}

char* obtenerPalabrasVistaAgregar(Vista* vista) {

	GtkEntry *entryAdd;
	entryAdd= GTK_ENTRY(gtk_builder_get_object(vista->builder, "entry_add"));
	return (char*)gtk_entry_get_text(entryAdd);	
}

char* obtenerPalabrasVistaEliminar(Vista* vista) {
	
	GtkEntry *entryDelete;
	entryDelete= GTK_ENTRY(gtk_builder_get_object(vista->builder, "entry_delete"));
	return (char*)gtk_entry_get_text(entryDelete);
}

void mostrarPalabraVista(Vista* vista, char* palabra) {
	
	GtkListStore *listStore;
	listStore= GTK_LIST_STORE(gtk_builder_get_object(vista->builder, "liststore_results"));
	GtkTreeIter iter;
	gtk_list_store_append(listStore, &iter);
	gtk_list_store_set(listStore, &iter, 0, palabra, -1);
}

void mostrarErrorVista(Vista* vista, char* mensaje) {
	
	GtkLabel *labelError;
	labelError= GTK_LABEL(gtk_builder_get_object(vista->builder, "label_error"));
	gtk_label_set_text(labelError, mensaje); 
}

void limpiarEntrys(Vista* vista) {
	
	GtkListStore *listStore;
	listStore= GTK_LIST_STORE(gtk_builder_get_object(vista->builder, "liststore_results"));
	gtk_list_store_clear(listStore);
	
	GtkEntry *entry;
	entry= GTK_ENTRY(gtk_builder_get_object(vista->builder, "entry_words"));
	gtk_entry_set_text(entry, "");
	
	entry= GTK_ENTRY(gtk_builder_get_object(vista->builder, "entry_add"));
	gtk_entry_set_text(entry, "");             
	
	entry= GTK_ENTRY(gtk_builder_get_object(vista->builder, "entry_delete"));
	gtk_entry_set_text(entry, "");   
	
	mostrarErrorVista(vista, "");          
}	

void limpiarTreeView(Vista* vista) {

	GtkListStore *listStore;
	listStore= GTK_LIST_STORE(gtk_builder_get_object(vista->builder, "liststore_results"));
	gtk_list_store_clear(listStore);	
}


void destruirVista(Vista* vista) { }
