// TMainWindow.cpp
#include <Application.h>
#include <Messenger.h>
#include <Message.h>
#include <Alert.h>
#include <Roster.h>
#include <Window.h>
#include <View.h>
#include <Rect.h>
#include <MenuBar.h>
#include <Menu.h>
#include <MenuField.h>
#include <TabView.h>
#include <Entry.h>
#include <Path.h>
#include <Box.h>
#include <MenuItem.h>
#include <TextView.h>
#include <TextControl.h>
#include <StringView.h>
#include <FilePanel.h>
#include <File.h>
#include <Entry.h>
#include <Path.h>
#include <ScrollView.h>
#include <OutlineListView.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "BEBApp.h"
#include "TMainWindow.h"


// Constructs the window we'll be drawing into.
//

TMainWindow::TMainWindow(BRect frame)
			: BWindow(frame, " ", B_TITLED_WINDOW, B_NOT_RESIZABLE|B_NOT_ZOOMABLE|B_NOT_CLOSABLE) {
	
	
   	
	BRect tabviewr=Bounds();
	BRect textframe = Bounds();
	
	menubar = new BMenuBar(r, "menu_bar");

	// Add File menu to menu bar
	menu = new BMenu("File");
	menu->AddItem(new BMenuItem("New", new BMessage(MENU_FILE_NEW), 'N'));
	menu->AddItem(item=new BMenuItem("Open" B_UTF8_ELLIPSIS,
					new BMessage(MENU_FILE_OPEN), 'O'));
	item->SetTarget(be_app);
	menu->AddSeparatorItem();
	menu->AddItem(saveitem=new BMenuItem("Save", new BMessage(MENU_FILE_SAVE), 'S'));
	saveitem->SetEnabled(false);
	//menu->AddItem(saveasitem=new BMenuItem("Save as" B_UTF8_ELLIPSIS,
	//				new BMessage(MENU_FILE_SAVEAS)));
	//saveasitem->SetEnabled(false);				
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Quit", new BMessage(MENU_FILE_QUIT), 'Q'));
	menubar->AddItem(menu);
	
	// Add Tools menu to menu bar
	menu = new BMenu("Tools");
	menu->AddItem(new BMenuItem("Author Index", new BMessage(MENU_TOOLS_AUTHOR), 'A'));
	menu->AddItem(new BMenuItem("Journal Index", new BMessage(MENU_TOOLS_JOURNAL), 'J'));
	//menu->AddItem(new BMenuItem("Reference Template", new BMessage(MENU_TOOLS_FORMAT), 'R'));
	menubar->AddItem(menu);
	
	// Add Help menu to menu bar
	menu = new BMenu("Help");
	menu->AddItem(new BMenuItem("Help", new BMessage(MENU_HELP_HELP), 'H'));
	menu->AddItem(new BMenuItem("About", new BMessage(MENU_HELP_ABOUT)));

	menubar->AddItem(menu);
	
	
	// Tab View
	tabviewr.top=r.bottom+21;
	refView = new BTabView(tabviewr, "tab_view"); 
	refView->SetViewColor(kGrayColor);
	tabviewr=refView->Bounds();
	tabviewr.InsetBy(5.0,5.0);
	tabviewr.bottom -=refView->TabHeight();
	
	// Data Browser Tab
	tab = new BTab();
	Browserview= new BView(tabviewr,"theview",B_FOLLOW_ALL,0);
	Browserview->SetViewColor(kGrayColor);
	
	// Browser Buttons
	float bline=tabviewr.bottom-60;
	FirstBtn=new BButton(BRect(0,bline,30,bline+5),"first", "<<",
	          new BMessage(BTN_FIRST_REQUESTED), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM, 
         	  B_WILL_DRAW | B_NAVIGABLE); 
   	Browserview->AddChild(FirstBtn); 
   	
   	PreviousBtn=new BButton(BRect(35,bline,65,bline+5),"previous", "<",
	          new BMessage(BTN_PREVIOUS_REQUESTED), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM, 
         	  B_WILL_DRAW | B_NAVIGABLE); 
   	Browserview->AddChild(PreviousBtn); 
   	
   	NextBtn=new BButton(BRect(70,bline,100,bline+5),"next", ">",
	          new BMessage(BTN_NEXT_REQUESTED), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM, 
         	  B_WILL_DRAW | B_NAVIGABLE); 
   	Browserview->AddChild(NextBtn); 
   	
   	LastBtn=new BButton(BRect(105,bline,135,bline+5),"last", ">>",
	          new BMessage(BTN_LAST_REQUESTED), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM, 
         	  B_WILL_DRAW | B_NAVIGABLE); 
   	Browserview->AddChild(LastBtn); 
   	
   	
   	CopyRefBtn=new BButton(BRect(35,bline-25,100,bline-10),"copy", "Copy",
	          new BMessage(BTN_COPYREF_REQUESTED), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM, 
         	  B_WILL_DRAW | B_NAVIGABLE); 
   	Browserview->AddChild(CopyRefBtn); 
   	
   	// a text for the record number
   	
   	RecNbrStr= new BTextControl(BRect(140,bline+3,190,bline+20), "recnbrstr", "","0", 
   					new BMessage(GOTO_REQUESTED), 
   					B_FOLLOW_LEFT | B_FOLLOW_BOTTOM,B_WILL_DRAW | B_NAVIGABLE);
   	RecNbrStr->SetDivider(5); 				 
	Browserview->AddChild(RecNbrStr);
   	
   	// Database Modification buttons
   	ClearBtn=new BButton(BRect(200,bline-25,250,bline-10),"Clear", "New",
	          new BMessage(BTN_CLEAR_REQUESTED), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM, 
         	  B_WILL_DRAW | B_NAVIGABLE); 
   	Browserview->AddChild(ClearBtn);  
   	
   	AddBtn=new BButton(BRect(200,bline,250,bline+5),"add", "Append",
	          new BMessage(BTN_ADD_REQUESTED), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM, 
         	  B_WILL_DRAW | B_NAVIGABLE); 
   	Browserview->AddChild(AddBtn); 
   	
   	DeleteBtn=new BButton(BRect(255,bline-25,305,bline-10),"del", "Delete",
	          new BMessage(BTN_DEL_REQUESTED), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM, 
         	  B_WILL_DRAW | B_NAVIGABLE); 
   	Browserview->AddChild(DeleteBtn); 	
	
	ModifyBtn=new BButton(BRect(255,bline,305,bline+5),"modify", "Modify",
	          new BMessage(BTN_MODIFY_REQUESTED), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM, 
         	  B_WILL_DRAW | B_NAVIGABLE); 
   	Browserview->AddChild(ModifyBtn); 
   	
   	SearchStr = new BTextControl(BRect(310,bline-20,tabviewr.right-20,bline-5), "Searchstr", "Find","String", 
   					new BMessage(BTN_FIND_REQUESTED), 
   					B_FOLLOW_LEFT | B_FOLLOW_BOTTOM,B_WILL_DRAW | B_NAVIGABLE);
   	SearchStr->SetDivider(40.00); 				 
	Browserview->AddChild(SearchStr);
   	
   	
   	SearchMenu = new BPopUpMenu("Matching Records");
   	BMenuField *ResuItem= new BMenuField(BRect(310,bline,tabviewr.right-20,bline+5), "search","Result",
    			SearchMenu,B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
   	ResuItem->SetDivider(42.00); 
     	
   	 
    Browserview->AddChild (ResuItem);
   
 	
   	  	
   
	// Data Fields and Controls
	// Document
	Docmenu = new BPopUpMenu("doc");
   	BMenuField *DocItem= new BMenuField(BRect(0,0,150,8), "docs","Document",
    			Docmenu,B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
   	DocItem->SetDivider(55.00); 
   	
   	// associate a Message to the  list 			
   	BMessage *DMes=new BMessage(DOCUMENT_REQUESTED);
   	
   	Docmenu->AddItem(item=new BMenuItem("Journal Article",new BMessage(DMes)));
    Docmenu->AddItem(item=new BMenuItem("Chapter",new BMessage(DMes)));
    Docmenu->AddItem(item=new BMenuItem("Book",new BMessage(DMes)));
    Docmenu->AddItem(item=new BMenuItem("Report",new BMessage(DMes)));
    Docmenu->AddItem(item=new BMenuItem("Thesis",new BMessage(DMes)));
    Docmenu->AddItem(item=new BMenuItem("Other",new BMessage(DMes)));
    
   // Go to First List Item
   item=(BMenuItem *)Docmenu->ItemAt(0);
   item->SetMarked(true);
   Browserview->AddChild (DocItem);
   
   // A textview for the document type
   DocStr= new BTextView(BRect(155,3,255,18), "docstr", BRect(0,0,100,10),
   					B_FOLLOW_LEFT|B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE); 
   DocStr->SetText(" "); 
   DocStr->MakeEditable(false);
   DocStr->MakeSelectable(false);
   Browserview->AddChild(DocStr);
   	
   	 
   // Authors Field
   
   BStringView *TmpStr= new BStringView(BRect(0,30,40,40), "tmpstr", "Authors", 
   					B_FOLLOW_LEFT|B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE); 
   Browserview->AddChild(TmpStr);
   
   AuthorStr= new BTextView(BRect(45,30,200,80), "AuthorStr", 
                            BRect(0,0,450,45),
   					B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE); 
   BScrollView *AScroll;					
   Browserview->AddChild(AScroll = new BScrollView("scroll_view", AuthorStr,
				B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP, 0, false, true, B_NO_BORDER));					
   
   // Authors Index List 
   AuthorMenu = new BPopUpMenu("author");
   AuthorItem= new BMenuField(BRect(280,0,560,60), "AuthorMenu","Author Index",
    			AuthorMenu, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
   AuthorItem->SetDivider(80.00); 
   	
     
   
   
   // Title Field
   
   BStringView *TmpStr0= new BStringView(BRect(220,30,250,40), "tmpstr1", "Title", 
   					B_FOLLOW_LEFT|B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE); 
   Browserview->AddChild(TmpStr0);
   
   TitleStr= new BTextView(BRect(255,30,tabviewr.right-45,80), "TitleStr", 
                            BRect(0,0,230,40),
   					B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE); 
   BScrollView *TScroll;					
   Browserview->AddChild(TScroll = new BScrollView("scroll_view", TitleStr,
				B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP, 0, false, true, B_NO_BORDER));
   
    // Book Field 
   BStringView *TmpStr1= new BStringView(BRect(0,90,30,100), "tmpstr0", "Book", 
   					B_FOLLOW_LEFT|B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE); 
   Browserview->AddChild(TmpStr1);
    
   BookStr= new BTextView(BRect(45,90,tabviewr.right-30,110), "BookStr", 
                            BRect(0,0,180,25),
   					B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE); 
   Browserview->AddChild(BookStr);
   
   // Journal Source Field
   BStringView *TmpStr2= new BStringView(BRect(0,130,40,140), "tmpstr2", "Source", 
   					B_FOLLOW_LEFT|B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE); 
   Browserview->AddChild(TmpStr2);
   
   SourceStr= new BTextView(BRect(45,130,245,160), "SourceStr", 
                            BRect(0,0,185,25),
   					B_FOLLOW_LEFT|B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE); 
   BScrollView *SScroll;					
   Browserview->AddChild(SScroll = new BScrollView("scroll_view", SourceStr,
			B_FOLLOW_LEFT|B_FOLLOW_TOP, 0, false, true, B_NO_BORDER));
   	
   		
   // Journal Index List
   
   JournalMenu = new BPopUpMenu("journal");
   JournalItem= new BMenuField(BRect(270,130,tabviewr.right-20,140), "JournalMenu","Journal Index",
    			JournalMenu, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
   JournalItem->SetDivider(70.00); 

  
   
   	
	
	// Year  volume  number and pages Fields
	YearStr = new BTextControl(BRect(0,170,120,180),
	                      "year","Year"," ", NULL,B_FOLLOW_LEFT|B_FOLLOW_TOP,
	                      B_WILL_DRAW|B_NAVIGABLE);
	YearStr->SetDivider(40);                      
	Browserview->AddChild(YearStr);
	VolumeStr = new BTextControl(BRect(125,170,245,180),
	                      "vol","Volume"," ", NULL,B_FOLLOW_LEFT|B_FOLLOW_TOP,
	                      B_WILL_DRAW|B_NAVIGABLE);
	VolumeStr->SetDivider(40);                      
	Browserview->AddChild(VolumeStr);
	
	
	NumberStr = new BTextControl(BRect(250,170,370,180),
	                      "no","Number"," ", NULL,B_FOLLOW_LEFT|B_FOLLOW_TOP,
	                      B_WILL_DRAW|B_NAVIGABLE);
	NumberStr->SetDivider(40);                      
	Browserview->AddChild(NumberStr);
	
	
	PageStr = new BTextControl(BRect(380,170,500,180),
	                      "page","Pages"," ", NULL,B_FOLLOW_LEFT|B_FOLLOW_TOP,
	                      B_WILL_DRAW|B_NAVIGABLE);
	PageStr->SetDivider(40);                      
	Browserview->AddChild(PageStr);
	
	// Series Editor, Publisher and Place
	
	EditorStr = new BTextControl(BRect(0,200,150,210),
	                      "editor","Editor"," ", NULL,B_FOLLOW_LEFT|B_FOLLOW_TOP,
	                      B_WILL_DRAW|B_NAVIGABLE);
	EditorStr->SetDivider(40);   
	Browserview->AddChild(EditorStr);
	
	
	PublisherStr = new BTextControl(BRect(160,200,350,210),
	                      "publisher","Publisher"," ", NULL,B_FOLLOW_LEFT|B_FOLLOW_TOP,
	                      B_WILL_DRAW|B_NAVIGABLE);
	PublisherStr->SetDivider(45);   
	Browserview->AddChild(PublisherStr);
	
	
	PlaceStr = new BTextControl(BRect(360,200,500,210),
	                      "place","Place"," ", NULL,B_FOLLOW_LEFT|B_FOLLOW_TOP,
	                      B_WILL_DRAW|B_NAVIGABLE);
	PlaceStr->SetDivider(40);   
	Browserview->AddChild(PlaceStr);
	
	// Keywords and Abstract Fields
	
	BStringView *TmpStr3= new BStringView(BRect(0,225,50,240), "tmpstr3", "Keywords", 
   					B_FOLLOW_LEFT|B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE); 
   	Browserview->AddChild(TmpStr3);
   
   	KwordStr= new BTextView(BRect(60,225,350,265), "KwordStr", 
                            BRect(0,0,155,35),
   					B_FOLLOW_LEFT|B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE); 
   	BScrollView *KScroll;					
   	Browserview->AddChild(KScroll = new BScrollView("scroll_view", KwordStr,
			B_FOLLOW_LEFT|B_FOLLOW_TOP, 0, false, true, B_NO_BORDER));
	
	
	BStringView *TmpStr4= new BStringView(BRect(0,275,50,285), "tmpstr4", "Abstract", 
   					B_FOLLOW_LEFT|B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE); 
   	Browserview->AddChild(TmpStr4);
   
   	AbstractStr= new BTextView(BRect(60,275,tabviewr.right-45,345), "AbstractStr", 
                            BRect(0,0,435,110),
   					B_FOLLOW_LEFT|B_FOLLOW_TOP,B_WILL_DRAW | B_NAVIGABLE); 
   	BScrollView *AbScroll;					
   	Browserview->AddChild(AbScroll = new BScrollView("scroll_view", AbstractStr,
			B_FOLLOW_LEFT|B_FOLLOW_TOP, 0, false, true, B_NO_BORDER));
	
	
	// Message textcontrol
	
	MessageStr= new BTextControl(BRect(10,tabviewr.bottom-30,tabviewr.right-10,tabviewr.bottom-25),
	                      "Message","Message "," ", NULL,B_FOLLOW_LEFT|B_FOLLOW_TOP,
	                      B_WILL_DRAW|B_NAVIGABLE);
	MessageStr->SetDivider(50);   
	Browserview->AddChild(MessageStr);
	MessageStr->SetEnabled(false);
	MessageStr->SetText("No Active Database");
	
	refView->AddTab(Browserview,tab); 
	tab->SetLabel("Browser");
	
	// Data Editor Tab
	tab = new BTab();
	Editorview= new BView(tabviewr,"theview",B_FOLLOW_ALL,0);
	Editorview->SetViewColor(kGrayColor);
	
	// Add the Editor text view objects
	// The Option Buttons
	
	SortBtn=new BButton(BRect(0,0,50,5),"sort", "Sort",
	          new BMessage(BTN_SORT_REQUESTED), B_FOLLOW_LEFT | B_FOLLOW_TOP, 
         	  B_WILL_DRAW | B_NAVIGABLE); 
   	Editorview->AddChild(SortBtn); 
   
   	CopyBtn=new BButton(BRect(55,0,105,5),"copy", "Copy to",
	          new BMessage(BTN_COPY_REQUESTED), B_FOLLOW_LEFT | B_FOLLOW_TOP, 
         	  B_WILL_DRAW | B_NAVIGABLE); 
   	Editorview->AddChild(CopyBtn); 
   
	  	
   	// The Editor TextView
	
	textframe.left +=2;
	textframe.top += 30;
	textframe.right -= (B_V_SCROLL_BAR_WIDTH*2);
	textframe.bottom -= (B_H_SCROLL_BAR_HEIGHT*5);
	
	BRect textrect = textframe;
	textrect.OffsetTo(B_ORIGIN);
	
	Editor = new BTextView(textframe, "text_view", textrect,
				B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_PULSE_NEEDED);
	Editorview->AddChild(EditorScroll = new BScrollView("scroll_view", Editor,
				B_FOLLOW_ALL_SIDES, 0, true, true, B_NO_BORDER));
	Editor->SetDoesUndo(true);
	
	refView->AddTab(Editorview,tab); 
	tab->SetLabel("Editor");
	
	
	
	// Attach the objects bar to the window
	AddChild(refView); // TabView
	AddChild(menubar); // Menu Bar
	
	// the main window controls the save panel
	savePanel = new BFilePanel(B_SAVE_PANEL, new BMessenger(this), NULL,
				B_FILE_NODE, false,NULL,NULL,true,true);
	SetTitle("RefLine");
	Show();
	Total_Number=0;
	Current_Record_Number=0;
}

void TMainWindow::SearchFor()
{
	char *text;
	int i;
	BMessage *MatchMes=new BMessage(SEARCH_REQUESTED);
	Lock();
	MessageStr->SetText("Search in progress...");
	strcpy(cmdline,"grep -i ");
	strcat(cmdline,"\'");
	strcat(cmdline, SearchStr->Text());
	strcat(cmdline,"\' ");
	strcat(cmdline, bindir);
	strcat(cmdline,"tmp*");
	strcat(cmdline,"|tr -d \'[A-Z] [a-z] /'");
	strcat(cmdline,"|cut -d: -f1|uniq|sort -n > ");
	strcat(cmdline,bindir);
	strcat(cmdline,"result.txt");
	
	
	if (system(cmdline)==0)
	{ 
		i= SearchMenu->CountItems();
		do {
		SearchMenu->RemoveItem(i);
		i--;
		} while (i>=0);	
	
		
		
		strcpy(cmdline,bindir);
   		strcat(cmdline,"result.txt");  
   	  	 
   		if (f = fopen(cmdline, "r"))
   		{
   		text = (char *) malloc(30);
   		strncpy(text," ", 30);
   		while (fgets(text,30,f)!= NULL)
   	   	   	{	
   	   	   	i=strlen(text);
 			text[i-1]='\0'; // replace End of Line by \0
   	   	   	SearchMenu->AddItem(item=new BMenuItem(text,new BMessage(MatchMes)));
 			
   			}
   		free(text);
   		fclose(f);
   	
		}
	
	    
   // Go to First List Item
   item=(BMenuItem *)SearchMenu->ItemAt(0);
   item->SetMarked(true);
   PostMessage(SEARCH_REQUESTED);
   
   }
   MessageStr->SetText("Done");
   Unlock();
	
}



int	TMainWindow::SplitBase(char *BaseName,char *AMessage)
// Call the gawk program to split the database in n records
{
	Lock();
	strcpy (cmdline,"gawk -f ");
	strcat (cmdline,bindir);
	strcat (cmdline,"splitrecords "); 
	strcat(cmdline,BaseName);
	strcat(cmdline," > ");
	strcat(cmdline, bindir);
	strcat(cmdline ,"nr.txt");
	MessageStr->SetText(AMessage);
	Unlock();
	return system(cmdline);
}

void TMainWindow::ShowTotal()
// Show the total number of records
{
	Lock();
	// How many records in the base
	char tempo[20];
	strcpy(cmdline,bindir);
	strcat(cmdline ,"nr.txt");
	f = fopen(cmdline, "r");
	fscanf(f,"%s",tempo);	
	fclose(f);
	strcpy(cmdline,"Database");
	strcat(cmdline," contains ");
	strcat(cmdline,tempo);
	strcat(cmdline," records");
	MessageStr->SetText(cmdline);
	Total_Number=atoi(tempo);
	Unlock();

}


void TMainWindow::OpenDataBase()
// Open the Database
{
	
	DeleteTmp();
	if (file.SetTo(&ref, B_READ_WRITE) == B_OK) 
	{
	BEntry entry(&ref, true);
	BEntry parent;
	entry_ref parent_ref;
	BPath path;
	entry.GetParent(&parent);		// Get parent directory
	entry.GetName(name);
	parent.GetRef(&parent_ref);
	entry.GetPath(&path);		// Create a pathname for the directory
	strcpy(fullname,"");
	parent.GetPath(&path);
	strcpy(dirname,path.Path());
	strcat(fullname,dirname);
	strcat(fullname,"/");
	strcat(fullname,name);
	SetTitle(fullname);
	
	ClearFields();
	
	if (SplitBase(fullname,"Be patient! Extracting records from the database")==0)
		{
		ShowTotal();
		Current_Record_Number=1;
		ReadCurrentRecord(Current_Record_Number);
		saveitem->SetEnabled(true);
		//saveasitem->SetEnabled(true);
		}
	
	}
	
}


void TMainWindow::ShowAuthorIndex(bool firstlist)
{
	char *text;
   	int i;
   	 
   	if (firstlist!=true)
   	// remove the list  
   	{ 
   	i= AuthorMenu->CountItems();
	do {
		AuthorMenu->RemoveItem(i);
		i--;
	} while (i>=0);	
   	}
   	
   	// associate a Message to the  author list 			
   	BMessage *AMes=new BMessage(AUTHOR_REQUESTED);
   	
   	
   	strcpy(cmdline,toolsdir);
   	strcat(cmdline,"author");   
   	if (f = fopen(cmdline, "r"))
   	{
   	text = (char *) malloc(30);
   	
   	while (fgets(text,30,f)!= NULL)
   		{	
 		i=strlen(text);
 		text[i-1]='\0'; // replace End of Line by \0
 		AuthorMenu->AddItem(item=new BMenuItem(text,new BMessage(AMes)));
 		
   	}
   	fclose(f);
   	free(text);
   	// Go to First List Item
   	item=(BMenuItem *)AuthorMenu->ItemAt(0);
   	item->SetMarked(true);
    }
    if (firstlist) Browserview->AddChild (AuthorItem);	
}


void TMainWindow::ShowJournalIndex(bool firstlist)
{
	
   char *text;
   int i;
   if (firstlist!=true)
   {
 	// remove the list  
   i= JournalMenu->CountItems();
	do {
		JournalMenu->RemoveItem(i);
		i--;
	} while (i>=0);
  }
  		
  // associate a Message to the journal list 	
   BMessage *JMes=new BMessage(JOURNAL_REQUESTED);
   strcpy(cmdline,toolsdir);
   strcat(cmdline,"journal");   
   	
   if (f = fopen(cmdline, "r"))
   {
   text = (char *) malloc(255);
   while (fgets(text,255,f)!= NULL)
   		{	
 		i=strlen(text);
 		text[i-1]='\0';
 		JournalMenu->AddItem(item=new BMenuItem(text,new BMessage(JMes)));
   		}
   	fclose(f);
   	free(text);
   	// Go to First List Item
    item=(BMenuItem *)JournalMenu->ItemAt(0);
    item->SetMarked(true);
    }
    if (firstlist) Browserview->AddChild (JournalItem);
}


void TMainWindow::UpdateIndex(char *AMessage, char *AFilename, char *ACodeString)
{ 
   Lock();
   // Updates author journal index
	MessageStr->SetText(AMessage);
	
	// copy the file to a temp file
	strcpy(cmdline,"cat ");
	strcat(cmdline,toolsdir);
	strcat(cmdline,AFilename);
	strcat(cmdline," >");
	strcat(cmdline,toolsdir);
	strcat(cmdline,"tmp");
	system(cmdline);
		
	// extract from the active database the string with the code
	// %A for author or %J pour Journal
	// then cut from the character 4 ->EOL (i.e delete %A or %J)
	strcpy(cmdline,"grep \'");
	strcat(cmdline,ACodeString);
	strcat(cmdline,"\' ");
	strcat(cmdline,fullname);
	strcat(cmdline," | cut -c 4->> ");
	strcat(cmdline,toolsdir);
	strcat(cmdline,"tmp");
	system(cmdline);
	
	// Sort the result and delete doublons 	
	strcpy(cmdline,"sort -u ");
	strcat(cmdline,toolsdir);
	strcat(cmdline,"tmp >");
	strcat(cmdline,toolsdir);
	strcat(cmdline,AFilename);
	system(cmdline);
	// delete temp file	
	strcpy(cmdline,"rm ");
	strcat(cmdline,toolsdir);
	strcat(cmdline,"tmp");
		
	MessageStr->SetText("done");
	Unlock();

}
 


void TMainWindow::NewDataBase()
// New database
{	
	DeleteTmp();
	SetTitle("RefLine");
	Total_Number=0;
	Current_Record_Number=0;
	RecNbrStr->SetText("0");
	saveitem->SetEnabled(true);
	//saveasitem->SetEnabled(true);
	ClearFields();
	MessageStr->SetText("Fill the reference and use the Append button");
	strcpy(fullname,"");
	strcat(fullname,dbasedir);
	strcat(fullname,"untitled");
	SetTitle(fullname);
	

}

void TMainWindow::ReadCurrentRecord(int32 nrec)
{
	// First show current record number
	sprintf(chaine,"%d",nrec); // current record number
	Lock();
	RecNbrStr->SetText(chaine);
	strcpy(activefilename,bindir); // Current File Name
	strcat(activefilename,"tmp");
	strcat(activefilename,chaine);
	Unlock();
	strcpy(AuthorBuffer,"");
	
	char code[2];
	char tt[2];
	char ccode='O';
	// now split the fields of the  current record
	strcpy (cmdline,"gawk -f ");
	strcat (cmdline,bindir);
	strcat (cmdline,"splitfields "); 
	strcat(cmdline,activefilename);
	strcat(cmdline," > ");
	strcat(cmdline, bindir);
	strcat(cmdline ,"nf.txt");
	// How many fields in the current record
	if (system(cmdline)==0)
	{
		char tempo[20];
		
		strcpy(cmdline,bindir);
		strcat(cmdline ,"nf.txt");
		f = fopen(cmdline, "r");
		fscanf(f,"%s",tempo);	
		fclose(f);
		Number_of_Fields=atoi(tempo);
	
	}
	
	Lock();
	ClearFields();
	
	BFile file; // to read the field
	
		
	//Number_of_Fields+1
	for (int i=2; i<Number_of_Fields+1; i++)
	{
	// find the file corresponding to the field
	sprintf(chaine,"%d",i);
	strcpy(currentfieldname,bindir);
	strcat(currentfieldname,"ftmp");
	strcat(currentfieldname,chaine);
	
	
	// read the field
	if (file.SetTo(currentfieldname, B_READ_ONLY) == B_OK) 
		{
		off_t length;
		char *text;
		file.GetSize(&length);		// Get the file length;
		text = (char *) malloc(length+1);
		if (length>0) strncpy(text," ", length);
		file.ReadAt(0,code,1);  // What kind of field
		file.ReadAt(1,tt,1);   // for % in abstract
		ccode=code[0];	
		if (text && (file.ReadAt(2,text,length-2) >= B_OK)) 
		{
		switch(ccode) {
			case '0':
			case 'O':DocStr->SetText(text); 
			break;
			
			case 'A':
			{
			
				AuthorStr->Insert(text);
				strncat(AuthorBuffer,text, strlen(text)-1);
				strcat(AuthorBuffer,", ");
			
			}
			break;
			case 'B': BookStr->SetText(text);
			break;
			case 'D':YearStr->SetText(text);
			break;
			case 'T':TitleStr->SetText(text);
			break;
			case 'J': SourceStr->SetText(text);
			break;
			case 'V': VolumeStr->SetText(text);
			break;
			case 'N':NumberStr->SetText(text);
			break;
			case 'P':PageStr->SetText(text);  
			break;
			case 'E':EditorStr->SetText(text);
			break;
			case 'I': PublisherStr->SetText(text); 
			break;
			case 'C':PlaceStr->SetText(text);
			break;
			case 'K': KwordStr->SetText(text);
			break;
			case 'X':AbstractStr->SetText(text);
			break;
			
			case 'F':
			case 'd':
			case 'R':
			case 'r':
			break;
			default:
			{
			// this is necessary for nn% strings that can be found in abstract
			int32 offset= AbstractStr->TextLength();
			int32 len=strlen(text);
			AbstractStr->Insert(offset,"% ",2,NULL);
			AbstractStr->Insert(offset+2,tt,1,NULL);
			AbstractStr->Insert(offset+3,text,len,NULL);
			
			}
			break;
								
		}	
			
		}
		
		free(text);
		
		}
			
	}
	
	
	Unlock();
}

void TMainWindow::FormatArticle()
// Article Format
//Authors. Article Title. Abbreviated Journal Title Date;Volume(Issue):Pages.
 
{
 	int32 len;
 	char *text;
    len=strlen(AuthorBuffer)+TitleStr->TextLength()+
        strlen(SourceStr->Text())+strlen(YearStr->Text())+
        strlen(VolumeStr->Text())+strlen(NumberStr->Text())+
        strlen(PageStr->Text());
        
    text = (char *) malloc(len+50);
 	strcpy(text,AuthorBuffer);
    strcat(text, ". ");
    strncat(text,TitleStr->Text(),TitleStr->TextLength()-1);
    strcat(text,". "); 
    strncat(text,SourceStr->Text(), SourceStr->TextLength()-1);
    strcat(text,". "); 
    strcat(text,YearStr->Text());
    strcat(text,";");
    strcat(text,VolumeStr->Text());
    strcat(text," ("); 
    strcat(text,NumberStr->Text());
    strcat(text,") : ");
    strcat(text,PageStr->Text());
 	strcat(text,".");
	Editor->Insert(text);
 	Editor->Insert("\n");
 	free(text);
 }

void TMainWindow::FormatChapter()
// Chapter format
//Chapter Authors. Chapter Title. Book Editor. Book Title. 
//Place of Publication: Publisher; Date. Pages.

{
    int32 len;
    char *text;
    len=strlen(AuthorBuffer)+TitleStr->TextLength()+
        strlen(EditorStr->Text())+BookStr->TextLength()+
        strlen(PlaceStr->Text())+strlen(PublisherStr->Text())+
        strlen(YearStr->Text())+strlen(PageStr->Text());
        
    text = (char *) malloc(len+50);
    strcpy(text,AuthorBuffer);
    strcat(text, ". ");
    strncat(text,TitleStr->Text(),TitleStr->TextLength()-1);
    strcat(text,". In: "); 
    strcat(text,EditorStr->Text());
    strcat(text,". "); 
    strncat(text,BookStr->Text(),BookStr->TextLength()-1 );
    strcat(text,". "); 
    strcat(text,PlaceStr->Text());
    strcat(text,": ");
    strcat(text,PublisherStr->Text());
    strcat(text,";");
    strcat(text,YearStr->Text());
    strcat(text,". p.");
 	strcat(text,PageStr->Text());
 	strcat(text,".");
	Editor->Insert(text);
 	Editor->Insert("\n");
 	free(text);

}

void TMainWindow::FormatBook()
// Book format
//Authors. Book Title. Place of Publication: Publisher; Date. Number of Pages.
{
    int32 len;
    char *text;
    len=strlen(AuthorBuffer)+BookStr->TextLength()+
        strlen(PlaceStr->Text())+strlen(PublisherStr->Text())+
        strlen(YearStr->Text())+strlen(PageStr->Text());
        
    text = (char *) malloc(len+50);
    strcpy(text,AuthorBuffer);
    strcat(text, ". ");
    strncat(text,BookStr->Text(),BookStr->TextLength()-1 );
    strcat(text,". "); 
    strcat(text,PlaceStr->Text());
    strcat(text,": ");
    strcat(text,PublisherStr->Text());
    strcat(text,";");
    strcat(text,YearStr->Text());
    strcat(text,". ");
    strcat(text,PageStr->Text());
 	strcat(text," p.");
 	Editor->Insert(text);
 	Editor->Insert("\n");
 	free(text);

}



void TMainWindow::CopyRef()
{
	char code;
	code= DocStr->ByteAt(0);
	switch(code)
	{
		case 'J':FormatArticle(); 
		break;
		case 'C':FormatChapter(); 
		break;
		case 'B':
		case 'R':
		case 'T':
		case 'O':
		FormatBook(); 
		break;
	}
			
	
}


void TMainWindow::WriteCurrentRecord(int32 nrec)
// write new or mofified reference
{
	int32 len;
	
	
	sprintf(chaine,"%d",nrec); // current record number
	RecNbrStr->SetText(chaine);
	strcpy(activefilename,bindir); // Current File Name
	strcat(activefilename,"tmp");
	strcat(activefilename,chaine);
	
	file.SetTo(activefilename, B_READ_WRITE|B_CREATE_FILE|B_ERASE_FILE);
	// Document
	file.Write("%O ",3);
	len=DocStr->TextLength(); 
	file.Write(DocStr->Text(),len);
		
	// Authors
	
	int32 start;
	int32 finish;
	
	for (int i=0;i<AuthorStr->CountLines();i++) 
	{
		AuthorStr->GoToLine(i);
		AuthorStr->GetSelection(&start,&finish);
		if (AuthorStr->ByteAt(start+1) !='\0') AuthorStr->Insert("%A ",NULL);
	}
	file.Write(AuthorStr->Text(),AuthorStr->TextLength());
	
	len= TitleStr->TextLength();
	if (len>0)
	{	
		file.Write("%T ",3);
		file.Write(TitleStr->Text(),TitleStr->TextLength());
	}
	
	len= BookStr->TextLength();
	if (len >0)
	{	file.Write("%B ",3);
		file.Write(BookStr->Text(),BookStr->TextLength());
	}
	
	len = SourceStr->TextLength();	
	if (len>0) 
	{	file.Write("%J ",3);
		file.Write(SourceStr->Text(),SourceStr->TextLength());
	}
	
	
	len= strlen(YearStr->Text());
	if (len >0) 
	{   file.Write("%D ",3);
		file.Write(YearStr->Text(),len);
		file.Write("\n",1);
	}
	
	len =strlen(VolumeStr->Text());
	if (len>0)
	{	file.Write("%V ",3);
		file.Write(VolumeStr->Text(),len);
		file.Write("\n",1);
	}	
	len = strlen(NumberStr->Text());
	if (len>0)
	{	file.Write("%N ",3);
		file.Write(NumberStr->Text(),len);
		file.Write("\n",1);
	}
	len = strlen(PageStr->Text());
	if (len>0)
	{	file.Write("%P ",3);
		file.Write(PageStr->Text(),len);
		file.Write("\n",1);
	}
	len= strlen(EditorStr->Text());
	if (len >0)
	{	file.Write("%E ",3);
		file.Write(EditorStr->Text(),len);
		file.Write("\n",1);
	}
	len = strlen(PublisherStr->Text());
	if (len>0)
	{	file.Write("%I ",3);
		file.Write(PublisherStr->Text(),len);
		file.Write("\n",1);
	}
	len = strlen(PlaceStr->Text());
	if (len >0)
	{	file.Write("%C ",3);
		file.Write(PlaceStr->Text(),len);
		file.Write("\n",1);
	}
	len = KwordStr->TextLength();
	if (len>0)
	{	file.Write("%K ",3);
		file.Write(KwordStr->Text(),KwordStr->TextLength());
	}
	len= AbstractStr->TextLength();
	if (len >0)
	{	file.Write("%X ",3);
		file.Write(AbstractStr->Text(),AbstractStr->TextLength());
	}
	
	file.Write("\n",1);	
	file.Unset();	
}

void TMainWindow::SaveFile()
{
	Lock();
	strcpy(cmdline, "Saving ");
	strcat(cmdline, fullname);
	MessageStr->SetText(cmdline);
	strcpy (cmdline,"cat ");
	strcat (cmdline,bindir);
	strcat (cmdline,"tmp* "); 
	strcat(cmdline," > ");
	strcat(cmdline, fullname);
	if (system(cmdline)==0)	MessageStr->SetText("File is saved");
	Unlock();
}


void TMainWindow::ClearFields()
// Clear all fields
{
	Lock();
	DocStr->SetText("");
	AuthorStr->SetText("");
	TitleStr->SetText("");
	BookStr->SetText("");
	SourceStr->SetText("");
	KwordStr->SetText("");
	AbstractStr->SetText("");
	YearStr->SetText("");
	VolumeStr->SetText("");
	NumberStr->SetText("");
	PageStr->SetText("");
	EditorStr->SetText("");
	PublisherStr->SetText("");
	PlaceStr->SetText("");
	Unlock();
}
	


void TMainWindow::DeleteTmp()
// del temporary files
{
		Lock();
		MessageStr->SetText("Deleting temporary files");
		
		strcpy(cmdline,"rm ");
		strcat(cmdline,bindir);
		strcat(cmdline,"tmp*");
		system(cmdline);
		
		strcpy(cmdline,"rm ");
		strcat(cmdline,bindir);
		strcat(cmdline,"ftmp*");
		system(cmdline);
		strcpy(cmdline,"rm ");
		strcat(cmdline,bindir);
		strcat(cmdline,"*.txt");
		system(cmdline);
		
		Unlock();		
}

void TMainWindow::DeleteCurrent()
// Delete the Current Record
{		
		// temporary file
		strcpy(tempofilename,bindir);
		strcat(tempofilename,"ftmp");
		
		strcpy(cmdline,"rm ");
		strcat(cmdline,activefilename); // Delete the current rcord file
 		system(cmdline);
		strcpy (cmdline,"cat ");	// Create the tempo file
		strcat (cmdline,bindir);
		strcat (cmdline,"tmp* "); 
		strcat(cmdline," > ");
		strcat(cmdline, tempofilename);
		system(cmdline);
		
}

void TMainWindow::SortReference()
//sort references in the Editor TextView
{
char	tempfilename[B_FILE_NAME_LENGTH+B_PATH_NAME_LENGTH+2];
BFile file; // to read and write the field
if (Editor->TextLength() >=1)
{
	strcpy(tempfilename,bindir);
	strcat(tempfilename,"tmpsort");
		f = fopen(tempfilename, "w");
	if (file.SetTo(tempfilename, B_READ_WRITE) == B_OK) 
	// write Editor to the file
	file.Write(Editor->Text(),Editor->TextLength()); 
	// sort the file in alpha order
	strcpy(cmdline,"sort ");
	strcat(cmdline, tempfilename);
	strcat(cmdline,"> ");
	strcat(cmdline,bindir);
	strcat(cmdline,"tmp2sort");
	system(cmdline);
	//remove tmpsort file;
	strcpy(cmdline,"rm ");
	strcat(cmdline,tempfilename);
	system(cmdline);
	// now read the sorted file to Editor
	strcpy(tempfilename,bindir);
	strcat(tempfilename,"tmp2sort");
	if (file.SetTo(tempfilename, B_READ_WRITE) == B_OK) 
	{
		char *text;
		off_t length;
		file.GetSize(&length);		// Get the file length;
		text = (char *) malloc(length);
		file.ReadAt(0,text,length);
		Editor->SetText(text);
		free(text); 
	
	}	
	//remove tmpsort file;
	strcpy(cmdline,"rm ");
	strcat(cmdline,tempfilename);
	system(cmdline);
}

}



// Respond to the B_QUIT_REQUESTED Message when the user closes the MainWindow
bool TMainWindow::QuitRequested() 
{
		
	// send this message to the app
	BMessage *quitmessage = new BMessage(MENU_FILE_QUIT);
	be_app->PostMessage(quitmessage);
	return true;
}


void TMainWindow::MessageReceived(BMessage *message) {

	switch(message->what) {
	case MENU_FILE_OPEN:	
	case MENU_FILE_QUIT:
	case MENU_HELP_ABOUT:
	case MENU_HELP_HELP	:
			be_app->PostMessage(message);
	break;
	
	
	case MENU_FILE_NEW: 
	{
	 NewDataBase();
	 
	 	}
	break;
	case MENU_FILE_SAVE	: SaveFile();
	break;
	
	case MENU_FILE_SAVEAS: 
		{
			savePanel->Show();
		}
		break;
	
	
	case DOCUMENT_REQUESTED: 
				
		{
			int32 anInt32;
			message->FindInt32("index", &anInt32);
			item=(BMenuItem *)Docmenu->ItemAt(anInt32);
			DocStr->SetText(item->Label());
			
		}	
			
		break;
	case AUTHOR_REQUESTED:
	{
			int32 anInt32;
			message->FindInt32("index", &anInt32);
			item=(BMenuItem *)AuthorMenu->ItemAt(anInt32);
			AuthorStr->Insert(item->Label());
			AuthorStr->Insert("\n");
	}
	break;
	case JOURNAL_REQUESTED:
	{
			int32 anInt32;
			message->FindInt32("index", &anInt32);
			item=(BMenuItem *)JournalMenu->ItemAt(anInt32);
			SourceStr->SetText(item->Label());
			SourceStr->Insert("\n");
	}
	break;		
	case BTN_FIRST_REQUESTED: if (Total_Number >0) 
		{Current_Record_Number=1;
		ReadCurrentRecord(Current_Record_Number);
	}
	break;
	case BTN_LAST_REQUESTED: if (Total_Number >0) 
		{Current_Record_Number=Total_Number;
		ReadCurrentRecord(Current_Record_Number);
	}
	break;
	case BTN_NEXT_REQUESTED: if (Total_Number >0) 
		{if (Current_Record_Number <Total_Number) Current_Record_Number++;
		ReadCurrentRecord(Current_Record_Number);
		}
	break;
	case BTN_PREVIOUS_REQUESTED:if (Total_Number >0) 
		{if (Current_Record_Number>1) Current_Record_Number--;
		ReadCurrentRecord(Current_Record_Number);
		}
	break;
	case GOTO_REQUESTED:
	if (Total_Number >0 )
	{
	{int32 rec;
	rec=atoi(RecNbrStr->Text());
	if (Total_Number >0 && rec <=Total_Number) ReadCurrentRecord(rec);
	Current_Record_Number=rec;
	}
	}
	break;
	
	case BTN_ADD_REQUESTED:
	
	{ Total_Number++;
	  Current_Record_Number=Total_Number;
	  Lock();
	  MessageStr->SetText("Appending a new reference");
	  Unlock();
	  WriteCurrentRecord(Current_Record_Number);
	  ReadCurrentRecord(Current_Record_Number);
		  	
	}
	break;
	case BTN_MODIFY_REQUESTED:
	{ 
	if (Total_Number >0 )
	{
	  Lock( );
	  MessageStr->SetText("Modifying the current reference");
	  Unlock();
	  WriteCurrentRecord(Current_Record_Number);
	  ReadCurrentRecord(Current_Record_Number);
	}
	}
	break;
	case BTN_DEL_REQUESTED:
	{
	int32 button_index;
	BAlert 			*alert;
	alert= new BAlert("Delete", "Do you want really delete this reference ?", "Yes","No");
	button_index = alert->Go();
	// If yes then Delete 
	if (button_index==0) 
	{
		int32 Deleted_Record_Number=Current_Record_Number;
		DeleteCurrent();
		ClearFields();
		if (SplitBase(tempofilename,"Be Patient ! Updating data")==0) 
		{Total_Number--;
		 ShowTotal();
		// goto the previous record
		if (Deleted_Record_Number>1)
		Current_Record_Number=Deleted_Record_Number-1;
		else Current_Record_Number=1;
		 ReadCurrentRecord(Current_Record_Number);
		} 
	}
	}
	
	break;
	
	case BTN_CLEAR_REQUESTED:if (Total_Number >0) 
	{
		ClearFields();
		Lock();
		MessageStr->SetText("Fill the form and then use the Append button");
		Unlock();
	}
	break;
	case BTN_COPYREF_REQUESTED: CopyRef();
	break;
	case BTN_COPY_REQUESTED:
	{
		Editor->SelectAll();
		Editor->Copy(be_clipboard);
	}
	break;
	case  BTN_SORT_REQUESTED: SortReference();
	break;
	case BTN_FIND_REQUESTED: if (Total_Number >0 ) SearchFor();
	break;
	case SEARCH_REQUESTED: 
	{
		int32 anInt32;
		message->FindInt32("index", &anInt32);
		item=(BMenuItem *)SearchMenu->ItemAt(anInt32);
		Current_Record_Number=atoi(item->Label());
		if (Current_Record_Number>0) ReadCurrentRecord(Current_Record_Number);
	}
	break;
	case MENU_TOOLS_AUTHOR:
	if (Total_Number >0 )
	{
		UpdateIndex("Updating Author Index","author","%A");
		ShowAuthorIndex(false);
		
	}
	break;
	case MENU_TOOLS_JOURNAL	:
	if (Total_Number >0 )
	{
		UpdateIndex("Updating Journal Index","journal","%J");
		ShowJournalIndex(false);
		
	}
	break;
	
}	
}






	