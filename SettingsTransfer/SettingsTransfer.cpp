#include <gtk/gtk.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <ShlObj.h>
#include <locale>
#include <codecvt>
#include <fstream>
#include <vector>

bool DEBUG = true;

/*this is the version of SettingsTransfer that attempts to use a struct instead of a class*/
const char *PROMPT1 = "STEP 1: To get started, select the \"options.txt\" file of the old minecraft version you \
want to transfer the settings from.";

const char *PROMPT2 = "STEP 2: Next, select the \"options.txt\" file of the new minecraft version you want to \
transfer the settings to. Make sure you have already installed the mods you want, and you have launched this\
version at least once. ";

const char *PROMPT3 = "STEP 3: Transfer Settings!";


struct appdata{
    GtkApplication* app;
    GtkWidget *window, *nextButton, *fixed, *backButton, *view, *nextLabel; 
    GtkFileDialog *fileDialog;
    GtkTextBuffer *buffer;
    GtkTextTagTable *tagTable;
    GtkTextTag *centerTag;
    GFile *defaultFile;

    int counter{1}, screenWidth, screenHeight, newScreenWidth, newScreenHeight, \
    nextButtonWidth, nextButtonHeight, textW, textH, backButtonW, backButtonH;
    double nextButtonX, nextButtonY, textX, textY, backButtonX, backbuttonY;
    bool started = false;

    std::string file1{""};
    std::string file2{""};
    std::string usrPath;
    std::string mcPath;

};

//#######################################################################################

void wrongFile(appdata *AppData){
    GtkAlertDialog *badFile = gtk_alert_dialog_new("Warning");
    gtk_alert_dialog_set_detail(badFile, "Selected file must be \"options.txt\"");
    gtk_alert_dialog_show(badFile, GTK_WINDOW(AppData->window)); 
}

//#######################################################################################

void open_file_dialog(GObject* source_object, GAsyncResult *res, gpointer user_data) {

    appdata *AppData = static_cast<appdata*>(user_data);

    if(::DEBUG) g_print("file successfully selected\n");

    GError* error = NULL;
    GFile* file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(AppData->fileDialog), res, &error);
    if(::DEBUG) g_print("it at least gets here\n");

    if(error) {
        if(::DEBUG) g_print("Error opening file: %s\n", error->message);
        g_error_free(error);
    }
    if (file) {
        if(::DEBUG) g_print("checkpoint 3\n");
        char* path = g_file_get_path(file);
        if(::DEBUG) g_print("checkpoint 3.5\n");

        if(AppData->counter == 1) { 
            if (path) {
                AppData->file1 = std::string(path);
                g_free(path);
                if(::DEBUG) g_print("File 1: %s\n", path);
                if(::DEBUG) std::cout << "String Version: " << AppData->file1 << std::endl;

                if(AppData->file1.ends_with("options.txt")){
                    if(::DEBUG) g_print("Counter: %i\n", AppData->counter);
                    AppData->counter++;
                    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(AppData->buffer), ::PROMPT2, -1);                    
                } else {
                    //makes a warning that the file must be options.txt
                    wrongFile(AppData);                   
                }
            }
        } else if(AppData->counter == 2){
            if (path) {
                AppData->file2 = std::string(path);
                g_free(path);
                if(::DEBUG) g_print("File 2: %s\n", path);
                if(::DEBUG) std::cout << "String Version: " << AppData->file2 << std::endl;

                if(AppData->file2.ends_with("options.txt")){
                    if(::DEBUG) g_print("Counter: %i\n", AppData->counter);
                    AppData->counter++;
                    gtk_label_set_text(GTK_LABEL(AppData->nextLabel), "Transfer Settings!");
                    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(AppData->buffer), ::PROMPT3, -1);                    
                } else {
                    //makes a warning that the file must be options.txt
                    wrongFile(AppData);                   
                }
            }

        } 
        g_object_unref(file); // Clean up the GFile object   
        } else {if(::DEBUG) g_print("No file selected.\n");}
              
    }

//#######################################################################################

int count(std::ifstream &input){
    std::string ctrstr;
    int ctr = 0;

    if(input){
        while(std::getline(input, ctrstr)){
            if(::DEBUG) std::cout << ctrstr << "\nThis next one is different:\n";
            ctr++;
        }
        if(::DEBUG) std::cout << "Lines in file:" << ctr << "\n";
    }

    return ctr;
}

//#######################################################################################

void mergeFiles(appdata *AppData){

    std::string tempstr;
    int i, colonAt;

    std::ifstream input1{AppData->file1, std::ios::in};
    std::ifstream input2{AppData->file2, std::ios::in};

    const int LEN1{count(input1)};
    const int LEN2{count(input2)};

    std::vector<std::vector<std::string>> file1(LEN1, std::vector<std::string>(2));
    std::vector<std::vector<std::string>> file2(LEN2, std::vector<std::string>(2));

    input1.clear();
    input1.seekg(0, std::ios::beg);
    input2.clear();
    input2.seekg(0, std::ios::beg);

    if(input1){
        
        if(::DEBUG) g_print("input1 valid\n");

        for(i = 0; std::getline(input1, tempstr); i++){
            
            if(tempstr.ends_with("\n")) tempstr.pop_back();
            colonAt = tempstr.find_first_of(':');
            if(::DEBUG) std::cout << 'i';

            file1.at(i).at(0) = tempstr.substr(0, colonAt);
            file1.at(i).at(1) = tempstr.substr(colonAt + 1);
        }

        if(::DEBUG) std::cout << "\nFirst item in vector file1: " << file1.at(0).at(0) << std::endl;

        if(input2){
        for(i = 0; std::getline(input2, tempstr) ; i++){
            
            if(tempstr.ends_with("\n")) tempstr.pop_back();
            colonAt = tempstr.find_first_of(':');
            
            file2.at(i).at(0) = tempstr.substr(0, colonAt);
            file2.at(i).at(1) = tempstr.substr(colonAt + 1);
            if(::DEBUG) std::cout << i << ':' <<  file2.at(i).at(0) << " & " << file2.at(i).at(1) << std::endl;
        }

            if(::DEBUG) std::cout << "\nFirst item in vector file2: " << file2.at(0).at(0) << std::endl;

            if(::DEBUG){
                
                for(auto& x: file1){
                    for(const std::string y: x){
                        std::cout << y << ':';
                    }
                    std::cout << std::endl;
                }
            }

            std::string version = "version";

            if(::DEBUG) std::cout << "\nCommence Stitching\n" << "File1 Version: " << file1.front().back()\
            << "\n" << "File2 Version: " << file2.front().back() <<std::endl;


            for(auto& x : file1){
                for(auto& y: file2){
                    if(x.at(0) == y.at(0)){
                        if (x.at(0) == version) continue;
                        else y.at(1) = x.at(1);
                                                    
                    }            
                }
            }
            if(::DEBUG){
                for(auto& x : file2){
                    if(::DEBUG) std::cout <<  x.at(0) << " & " << x.at(1) << std::endl;
                }
            }
            std::ofstream output{AppData->file2, std::ios::out};
            for(auto& x : file2){
                output << x.at(0) + ":" + x.at(1) << "\n";
            }
        }
    }
}

//#######################################################################################

static void onButtonPress(GtkWidget *widget, gpointer user_data) {

    appdata *AppData = static_cast<appdata*>(user_data);

    AppData->fileDialog = gtk_file_dialog_new();
    gtk_file_dialog_set_initial_folder(AppData->fileDialog, AppData->defaultFile);    

    if(AppData->counter == 1) {
        if(::DEBUG) g_print("option 1\n");
        gtk_file_dialog_open(AppData->fileDialog, GTK_WINDOW(AppData->window), NULL, open_file_dialog, user_data);
    }

    if(AppData->counter == 2) {
        if(::DEBUG) g_print("option 2\n");       
        gtk_file_dialog_open(AppData->fileDialog, GTK_WINDOW(AppData->window), NULL, open_file_dialog, user_data);
    }

    if(AppData->counter == 3) {
        if(::DEBUG) g_print("yay 3\n");
        mergeFiles(AppData);
    }
}

//#################################################################################################

static void onBack(GtkWidget *widget, gpointer user_data){
    appdata *AppData = static_cast<appdata*>(user_data);

    if (AppData->counter > 1) AppData->counter--;
    if (AppData->counter == 1) {
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(AppData->buffer), ::PROMPT1, -1);
        AppData->file1.clear();
        if(::DEBUG) std::cout << "File 1: " << AppData->file1 << "\n" << std::endl; 
    }
    else if (AppData->counter == 2){
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(AppData->buffer), ::PROMPT2, -1);
        AppData->file2.clear();
        if(::DEBUG) std::cout << "File 2: " << AppData->file2 << "\n" << std::endl;
    }
}

//##########################################################################################

gboolean updateWidgets(gpointer user_data){
    appdata *AppData = static_cast<appdata*>(user_data);

    gtk_window_get_default_size(GTK_WINDOW(AppData->window), &AppData->newScreenWidth, &AppData->newScreenHeight);
    if (AppData->newScreenWidth != AppData->screenWidth || AppData->newScreenHeight != AppData->screenHeight){

        if(::DEBUG) g_print("screen size is not the same\n");
        AppData->nextButtonWidth = AppData->newScreenWidth / 10;
        AppData->nextButtonHeight = AppData->newScreenHeight / 10;
        AppData->nextButtonX = (AppData->newScreenWidth * .5) - (.5 * AppData->nextButtonWidth);
        AppData->nextButtonY = (AppData->newScreenHeight * .6) - (.5 * AppData->nextButtonHeight);
        if(!AppData->started){
            gtk_fixed_put(GTK_FIXED(AppData->fixed), AppData->nextButton, AppData->nextButtonX, AppData->nextButtonY);
        } else gtk_fixed_move(GTK_FIXED(AppData->fixed), AppData->nextButton, AppData->nextButtonX, AppData->nextButtonY);
        gtk_widget_set_size_request(AppData->nextButton, AppData->nextButtonWidth, AppData->nextButtonHeight);

        if(::DEBUG) g_print("screen size is not the same\n");
        AppData->backButtonW = AppData->newScreenWidth / 20;
        AppData->backButtonH = AppData->newScreenHeight / 20;
        AppData->backButtonX = (AppData->newScreenWidth * .025);
        AppData->backbuttonY = (AppData->newScreenHeight * .025);
        if(!AppData->started){
            gtk_fixed_put(GTK_FIXED(AppData->fixed), AppData->backButton, AppData->backButtonX, AppData->backbuttonY);
        } else gtk_fixed_move(GTK_FIXED(AppData->fixed), AppData->backButton, AppData->backButtonX, AppData->backbuttonY);
        gtk_widget_set_size_request(AppData->backButton, AppData->backButtonW, AppData->backButtonH);                 

        AppData->textW = .9 * AppData->newScreenWidth;
        AppData->textH = .15 * AppData->newScreenHeight;
        AppData->textX = (AppData->newScreenWidth * .5) - (.5 * AppData->textW);
        AppData->textY = (AppData->newScreenHeight * .4) - (.5 * AppData->textH);
        if(!AppData->started){
            gtk_fixed_put(GTK_FIXED(AppData->fixed), AppData->view, AppData->textX, AppData->textY);
            AppData->started = true;
        } else gtk_fixed_move(GTK_FIXED(AppData->fixed), AppData->view, AppData->textX, AppData->textY);
        gtk_widget_set_size_request(AppData->view, AppData->textW, AppData->textH);

        AppData->screenWidth = AppData->newScreenWidth;
        AppData->screenHeight = AppData->newScreenHeight;
    }
    
    return TRUE;   
}

//#######################################################################################

std::string getUserPath(){
    wchar_t path[MAX_PATH];

    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path))) {
        std::wstring ws(path);
        std::string userPath(ws.begin(), ws.end());
        if (::DEBUG) std::cout << "User profile path: " << userPath << std::endl;
        return userPath;
    } else {
        if(::DEBUG) std::cerr << "Failed to get user profile path" << std::endl;
    }

    return "";
}

//#######################################################################################

static void activate(GtkApplication *app, gpointer user_data){
    
    appdata *AppData = static_cast<appdata*>(user_data);
    GtkCssProvider *provider;
    GtkStyleContext *context;
    GtkTextTag *tag;

    //set up initial screen size
    AppData->screenWidth = GetSystemMetrics(SM_CXSCREEN) / sqrt(2);
    AppData->screenHeight = GetSystemMetrics(SM_CYSCREEN) / sqrt(2);
    if(::DEBUG) std::cout << "things initialized" << "\nScreenWidth: " << AppData->screenWidth << std::endl;

    //create a window
    AppData->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(AppData->window), "MC Settings Transfer");
    gtk_window_set_default_size( GTK_WINDOW(AppData->window), AppData->screenWidth, AppData->screenHeight);

    //create a fixed
    AppData->fixed = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(AppData->window), AppData->fixed);

    //create a textview and a buffer
    if(::DEBUG) g_print("before textview\n");
    AppData->view = gtk_text_view_new();
    AppData->buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(AppData->view));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(AppData->view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(AppData->view), FALSE);
    gtk_text_buffer_set_text(AppData->buffer, ::PROMPT1, -1);
    gtk_text_view_set_justification(GTK_TEXT_VIEW(AppData->view), GTK_JUSTIFY_CENTER);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(AppData->view), GTK_WRAP_WORD);

    //change textview font and color
    provider = gtk_css_provider_new();
    //use gtk_css_provider_load_from_string
    gtk_css_provider_load_from_string(provider,
                                 "textview {"
                                 " font: 25px serif;"
                                 "  color: black;"
                                 "}");
    context = gtk_widget_get_style_context(AppData->view);
    gtk_style_context_add_provider(context,
                                GTK_STYLE_PROVIDER (provider),
                                GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    //create the next button and label
    if(::DEBUG) g_print("before next button\n");
    AppData->nextButton = gtk_button_new();
    AppData->nextLabel = gtk_label_new("Select File");
    gtk_button_set_child(GTK_BUTTON(AppData->nextButton), AppData->nextLabel);
    gtk_label_set_wrap(GTK_LABEL(AppData->nextLabel), GTK_WRAP_WORD);

    AppData->backButton = gtk_button_new_with_label("Back");

    //update dimensions and position of all widgets
    gtk_window_set_default_size( GTK_WINDOW(AppData->window), AppData->screenWidth + 1, AppData->screenHeight + 1);
    updateWidgets(user_data);    
    
    g_signal_connect(AppData->nextButton, "clicked", G_CALLBACK(onButtonPress), user_data);
    g_signal_connect(AppData->backButton, "clicked", G_CALLBACK(onBack), user_data);
    
    if(::DEBUG) g_print("before presenting window\n");
    gtk_window_present(GTK_WINDOW(AppData->window));
    g_timeout_add(30, updateWidgets, user_data);

    AppData->usrPath = getUserPath();
    AppData->mcPath = AppData->usrPath + "\\AppData\\Roaming\\.minecraft";
    if(::DEBUG) std::cout << "mcPath: " << AppData->mcPath << std::endl;

    AppData->defaultFile = g_file_new_for_path(AppData->mcPath.c_str());
    if(::DEBUG) g_print("G mc File: %s\n", g_file_get_path(AppData->defaultFile));
 }

 //#######################################################################################


int main (int argc, char **argv){

    static appdata AppData;
    appdata *AppDataPtr = &AppData;

    if(::DEBUG) g_print("after defining struct\n");
    int status;
    if(::DEBUG) g_print("before new app\n");
    
    AppData.app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    if(::DEBUG) g_print("new app successful\n");
    g_signal_connect(AppData.app, "activate", G_CALLBACK (activate), AppDataPtr);
    if(::DEBUG) g_print("main runs\n");

    status = g_application_run (G_APPLICATION(AppData.app), argc, argv);

    g_object_unref(AppData.app);

    return status;
}
