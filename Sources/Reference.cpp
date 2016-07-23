//
// BEBApp.cpp
//

#include <Application.h>
#include <Messenger.h>
#include <Message.h>
#include <Alert.h>
#include <Roster.h>
#include <Window.h>
#include <TabView.h>
#include <View.h>
#include <MenuBar.h>
#include <Menu.h>
#include <MenuItem.h>
#include <FilePanel.h>
#include <Path.h>
#include <Directory.h>
#include <FindDirectory.h>
#include <Entry.h>
#include <TextView.h>
#include <TextControl.h>
#include <ScrollView.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "BEBApp.h"
#include "TMainWindow.h"

// Application's signature

const char *APP_SIGNATURE				= "application/x-vnd.Be-RefLine";

BRect windowRect(50,50,590,530);
TMainWindow		*reference_window;   // an instance of the main window
//
// The constructor for the BEApp class.  This
// will create the main window.
//
BEBApp::BEBApp()
			: BApplication(APP_SIGNATURE) {
	
	next_untitled_number = 1;	// Next window is "Untitled 1"
	reference_window=new TMainWindow(windowRect);
	// open panel must be modal
	reference_window->openPanel = new BFilePanel(B_OPEN_PANEL,NULL,NULL,0,true,
	                            NULL,NULL,true,true);
	ReadIni();  // Read config file 
	reference_window->ShowAuthorIndex(true);
	reference_window->ShowJournalIndex(true);                  
}

//
// BEBApp::MessageReceived
//
// Handle incoming messages.  
//
void BEBApp::MessageReceived(BMessage *message) 
{
switch(message->what) 
	{
	case MENU_FILE_QUIT:
	{
	int32 button_index;
	BAlert 			*alert;
	alert= new BAlert("Quit", "Do you want really quit References ?", "Yes","No");
	button_index = alert->Go();
	// If yes then Quit 
	if (button_index==0) 
	{
		reference_window->DeleteTmp();
		Quit();
	}
	
	}
	break;
	case MENU_FILE_OPEN:
			reference_window->openPanel->Show();		// Show the file panel
	break;	
	case MENU_HELP_ABOUT:
		{
			BAlert		*alert;
			alert= new BAlert("About ", "RefLine for BeOS™\n© F. Jouen 1999.\ne-mail:jouen@epeire.univ-rouen.fr", "OK");
			alert->Go();
		}	
		break;
	case MENU_HELP_HELP: system(helpfilename); 
	break;
			
	default:
	BApplication::MessageReceived(message);
	break;
	
		
	}	
	
}


// Handle a refs received message.
//
void BEBApp::RefsReceived(BMessage *message) 
{
    int32 ref_num ;

    ref_num=0;   
    if ( message->FindRef("refs", ref_num, &reference_window->ref) == B_OK ) 
    { 	
    	reference_window->OpenDataBase(); 
    	
     }      

}

void BEBApp::ReadIni()
// read ini file for application paths
{
	FILE *f;
	BDirectory *userdir = new BDirectory();
	BEntry		*entry = new BEntry;
	BPath		path;
    
    // Application setup file 
	find_directory(B_USER_SETTINGS_DIRECTORY, &path, true);
	userdir->SetTo(path.Path());	
	
	if (userdir->FindEntry("RefLine_data", entry) == B_OK) 
	{
	entry->GetPath(&path); 
    f=fopen(path.Path(),"r");
	fscanf(f,"%s",reference_window->appdir);	
   	fscanf(f,"%s",reference_window->toolsdir);
   	fscanf(f,"%s",reference_window->bindir);
   	fscanf(f,"%s",reference_window->dbasedir);	
 	fclose(f);
 	strcpy(helpfilename,"StyledEdit ");
	strcat(helpfilename,reference_window->toolsdir);
	strcat(helpfilename,"help");
	}
	else
	// default paths 
	{
	strcpy(reference_window->appdir,"/boot/home/Reference/");
	strcpy(reference_window->toolsdir,"/boot/home/Reference/Tools/");
	strcpy(reference_window->bindir,"/boot/home/Reference/Bin/");
	strcpy(reference_window->dbasedir,"/boot/home/Reference/Databases/");
	strcpy(helpfilename,"StyledEdit /boot/home/Reference/Tools/help");
	}
}


//
// main
//
// The main() function's only real job in a basic BeOS
// application is to create the BApplication object
// and run it.
//
int main(void) {
	BEBApp theApp;		// The application object
	theApp.Run();
	return 0;
}



