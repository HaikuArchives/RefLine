// BEApp.h
extern const char *APP_SIGNATURE;

class BEBApp : public BApplication {
	public:
						BEBApp();
		virtual void	MessageReceived(BMessage *message);
		virtual void	RefsReceived(BMessage *message);
		void			ReadIni();
	
	private:
		int32			next_untitled_number;
		char			helpfilename[B_FILE_NAME_LENGTH+B_PATH_NAME_LENGTH+2];
		
};
