//MainWindow.h
class TMainWindow : public BWindow {
	public:
	TMainWindow(BRect frame);
	char			cmdline[255];
	char 			appdir[B_PATH_NAME_LENGTH];
	char 			toolsdir[B_PATH_NAME_LENGTH];
	char 			bindir[B_PATH_NAME_LENGTH];
	char 			dbasedir[B_PATH_NAME_LENGTH];
	BFilePanel		*openPanel;
	BFilePanel		*savePanel;
	entry_ref 	ref;		// The entry_ref to open
	BTextControl	*MessageStr;
	virtual void	MessageReceived(BMessage *message);	
	virtual bool	QuitRequested();	
	void 	OpenDataBase();	
	void 	DeleteTmp();
	void	ShowAuthorIndex(bool firstlist);
	void	ShowJournalIndex(bool firstlist);
			
	private:					
			BMenuBar		*menubar;
			BRect 			r;
			BMenu 			*menu;
			BMenu 			*Docmenu;
			BMenu			*AuthorMenu;
			BMenu			*JournalMenu;
			BMenu			*SearchMenu;
			BMenuItem 		*item;
			BMenuItem 		*saveitem; 
			BMenuItem 		*saveasitem; 
			BMenuField 		*JournalItem;
			BMenuField 		*AuthorItem;
			BTabView 		*refView;
			BView 			*Browserview;
			BView 			*Editorview;
			BTab 			*tab; 
			BTextView		*Editor;
			BScrollView 	*EditorScroll;
			BTextControl	*RecNbrStr;
			BTextControl	*SearchStr;
			BTextView 		*DocStr;
			BTextView 		*AuthorStr;
			BTextView 		*TitleStr;
			BTextView		*BookStr;
			BTextView 		*SourceStr;
			BTextView		*KwordStr;
			BTextView		*AbstractStr;
			BTextControl	*YearStr;
			BTextControl	*VolumeStr;
			BTextControl	*NumberStr;
			BTextControl	*PageStr;
			BTextControl	*EditorStr;
			BTextControl	*PublisherStr;
			BTextControl	*PlaceStr;
			BButton			*SortBtn;
			BButton			*FormatBtn;
			BButton			*PrintBtn;
			BButton			*SaveBtn;
			BButton			*CopyBtn;
			BButton			*FirstBtn;
			BButton			*PreviousBtn;
			BButton			*NextBtn;
			BButton			*LastBtn;
			BButton			*ClearBtn;
			BButton			*AddBtn;
			BButton			*DeleteBtn;
			BButton			*ModifyBtn;
			BButton			*CopyRefBtn;
			BFile 			file;
			int32			Total_Number;
			int32			Current_Record_Number;
			int32			Number_of_Fields; 
			// for the filename
			char 			name[B_FILE_NAME_LENGTH];	// For the filename
			char 			dirname[B_PATH_NAME_LENGTH];	// For the filename
			char 			fullname[B_FILE_NAME_LENGTH+B_PATH_NAME_LENGTH+2];
			char			activefilename[B_FILE_NAME_LENGTH+B_PATH_NAME_LENGTH+2];
			char			currentfieldname[B_FILE_NAME_LENGTH+B_PATH_NAME_LENGTH+2];
			char			tempofilename[B_FILE_NAME_LENGTH+B_PATH_NAME_LENGTH+2];
			char 			chaine[20];
			char			AuthorBuffer[2048];
			FILE 			*f;  //Posix File 
			// private methods

			void 	NewDataBase();	
			void 	ClearFields();
			void	ReadCurrentRecord(int32 nrec);
			void	WriteCurrentRecord(int32 nrec);
			void	DeleteCurrent();
			void	SaveFile();
			int		SplitBase(char *BaseName,char *AMessage);
			void	ShowTotal(); 
			void	FormatArticle();
			void	FormatChapter();
			void 	FormatBook();
			void 	CopyRef();
			void 	SearchFor();
			void	UpdateIndex(char *AMessage, char *AFilename, char *ACodeString); 
			void	SortReference();
													
};						