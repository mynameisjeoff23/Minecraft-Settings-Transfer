#include <gtk/gtk.h>
#include <windows.h>
#include <string>
#include <iostream>

int counter{1};
std::string file1;
struct File1 {
    std::string selected_file1;
};

struct File2 {
    std::string selected_file2;
};

void open_file_dialog(GObject* source_object, GAsyncResult *res, gpointer user_data) {

    g_print("file successfully selected\n");


    if(::counter == 1) {
        GError* error = NULL;
        GFile* file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(source_object), res, &error);
        g_print("it at least gets here\n");

        if(error) {
            g_print("Error opening file: %s\n", error->message);
            g_error_free(error);
        }
        if (file) {
            g_print("checkpoint 3\n");
            char* path = g_file_get_path(file);

            g_print("checkpoint 3.5\n");
            if (path) {
                g_print("checkpiont 4\n");
                ::file1 = std::string(path);
                g_print("checkpoint 4.5\n");
                g_free(path);
                g_print("checkpoint 4.7");
                g_print("Selected file: %s\n", path);
                g_print("string version%s\n", ::file1);
                ::counter++;
            }
            g_object_unref(file);  // Clean up the GFile object
        } else {
            g_print("No file selected.\n");
        }
    
    }
}

static void onButtonPress(GtkWidget *widget, gpointer user_data) {
    //static std::string file1;
    //static std::string file2;

    GtkWindow* window = GTK_WINDOW(user_data);
    GtkFileDialog* dialog = gtk_file_dialog_new();
    

    g_print("Hello World\n");

    if(::counter == 1) {
        g_print("option 1\n");
        gtk_file_dialog_open(dialog, window, NULL, open_file_dialog, user_data);
    }

    if(::counter == 2) {

    }

    if(::counter == 3) {

    }
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

    double buttonX = (screenWidth * .5) - (.5 * buttonWidth);
    double buttonY = (screenHeight * .6) - (.5 * buttonHeight);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "MC Settings Transfer");
    gtk_window_set_default_size( GTK_WINDOW(window), screenWidth, screenHeight);

    fixed = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(window), fixed);

    button = gtk_button_new_with_label("Select File");
    gtk_fixed_put(GTK_FIXED(fixed), button, buttonX, buttonY);
    gtk_widget_set_size_request(button, buttonWidth, buttonHeight);    
    
    g_signal_connect(button, "clicked", G_CALLBACK(onButtonPress), NULL);

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
