#include <gtk/gtk.h>
#include <windows.h>
//making some changes to test git
static void print_hello(GtkWidget *widget, gpointer data){
    g_print("Hello World\n");
}

/* there will be no button updating until I figure out a way to detect a resize that doesn't crash the program
void update_button_size(GtkWidget *button, GdkRectangle *allocation, gpointer data){
    int windowWidth = allocation->width;
    int windowHeight = allocation->height;

    int buttonWidth = windowWidth / 10;
    int buttonHeight = windowHeight / 10; 

    gtk_widget_set_size_request(button, buttonWidth, buttonHeight);
}

void updateButtonPlacement(GtkWidget *button, GtkWidget *fixed, GdkRectangle *allocation, gpointer data){
    int windowWidth = allocation->width; 
    int windowHeight = allocation->height;

    int buttonX = windowWidth * .6;
    int buttonY = windowHeight * .5;

    gtk_fixed_put(GTK_FIXED(fixed), button, buttonX, buttonY);
} */
static void activate(GtkApplication *app, gpointer user_data){
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *fixed;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN) / sqrt(2);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN) / sqrt(2);
    
    int buttonWidth = screenWidth / 10;
    int buttonHeight = screenHeight / 10;

    double buttonX = screenWidth * .5 - .5 * buttonWidth;
    double buttonY = screenHeight * .6 - .5 * buttonHeight;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "MC Settings Transfer");
    gtk_window_set_default_size( GTK_WINDOW(window), screenWidth, screenHeight);
    

    fixed = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(window), fixed);

    button = gtk_button_new_with_label("Hello Bitch");
    gtk_fixed_put(GTK_FIXED(fixed), button, buttonX, buttonY);
    gtk_widget_set_size_request(button, buttonWidth, buttonHeight);    
    
    
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

    gtk_window_present(GTK_WINDOW(window));
    
/*causes program to crash, won't be implemented yet, or maybe ever

    g_signal_connect(G_OBJECT(window), "notify::default-width", G_CALLBACK(updateButtonPlacement), NULL);    
    g_signal_connect(G_OBJECT(window), "notify::default-height", G_CALLBACK(updateButtonPlacement), NULL);
    g_signal_connect(G_OBJECT(window), "notify::default-width", G_CALLBACK(update_button_size), NULL);    
    g_signal_connect(G_OBJECT(window), "notify::default-height", G_CALLBACK(update_button_size), NULL); */  
}


int main (int argc, char **argv){
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);

    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref(app);

    return status;
}
