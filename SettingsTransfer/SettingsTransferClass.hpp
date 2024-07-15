#include <gtk/gtk.h>
#include <windows.h>
#include <string>
#include <iostream>

class SettingsTransfer{
    public: 

        static SettingsTransfer &getInstance(){
            static SettingsTransfer instance; // Guaranteed to be destroyed and instantiated on first use
            return instance;
        }

        //delete copy constructor and assignment operator
        SettingsTransfer(const SettingsTransfer&) = delete;
        SettingsTransfer& operator=(const SettingsTransfer&) = delete;

        void setWindow(GtkWidget *obj){window = obj;}

        GtkWidget *getWindow(){return window;}

        void setApp(GtkApplication *obj){app = GTK_APPLICATION(obj);}

        GtkApplication *getApp(){return app;}

        void setNextButton(GtkWidget *obj){nextButton = obj;}

        GtkWidget *getNextButton(){return nextButton;}

        void setFixed(GtkWidget *obj){fixed = obj;}

        GtkWidget *getFixed(){return fixed;}

        void setBackButton(GtkWidget *obj){backButton = obj;}

        GtkWidget *getBackButton(){return backButton;}

        void setFile1(std::string obj){file1 = obj;}

        std::string getFile1(){return file1;}

        void setFile2(std::string obj){file2 = obj;}

        std::string getFile2(){return file1;}

        double getNextButtonX(){return nextButtonX;}

        double getNextButtonY(){return nextButtonY;}

        void updateButtons(){
            gtk_window_get_default_size(GTK_WINDOW(window), &screenWidth, &screenHeight);
            nextButtonWidth = screenWidth / 10;
            nextButtonHeight = screenHeight / 10;
            gtk_widget_set_size_request(nextButton, nextButtonWidth, nextButtonHeight); 

            nextButtonX = (screenWidth * .5) - (.5 * nextButtonWidth);
            nextButtonY = (screenHeight * .6) - (.5 * nextButtonHeight);
            gtk_fixed_put(GTK_FIXED(fixed), nextButton, nextButtonX, nextButtonY);
        }

        void addCounter(){if(counter < 3) counter++;}

        void subCounter(){if(counter > 1) counter--;}

        int getCounter(){return counter;}

        void setScreenSize(int height, int width){//doesnt really work here
            screenHeight = height;
            screenWidth = width;
        }

        int getScreenWidth(){return screenWidth;}

        int getScreenHeight(){return screenHeight;}

        void setDialog(GtkFileDialog *dialog){fileDialog = dialog;}

        GtkFileDialog* getDialog(){return fileDialog;}

        
     
        GtkApplication* app;
        GtkWidget *window, *nextButton, *fixed, *backButton; 
        GtkFileDialog *fileDialog;

    private: 

        SettingsTransfer(){
            screenWidth = GetSystemMetrics(SM_CXSCREEN) / sqrt(2);
            screenHeight = GetSystemMetrics(SM_CYSCREEN) / sqrt(2);
            std::cout << "things initialized" << std::endl;
            nextButtonWidth = screenWidth / 10;
            nextButtonHeight = screenHeight / 10;
            nextButtonX = (screenWidth * .5) - (.5 * nextButtonWidth);
            nextButtonY = (screenHeight * .6) - (.5 * nextButtonHeight);
        };

        int counter{1}, screenWidth, screenHeight, nextButtonWidth, nextButtonHeight;
        double nextButtonX, nextButtonY;

        std::string file1{""};
        std::string file2{""};
};