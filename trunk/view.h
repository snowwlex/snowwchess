/*
 * view.h
 *
 *
 *      Author: snowwlex
 */

#ifndef VIEW_H_
#define VIEW_H_

class CLIView {
	protected:
		Model *myModel;
		WINDOW *myWindow;
		int myColor;
	public:
		CLIView(int height,int width,int y,int x, int _color, bool scroll = false, Model *model = 0);
		virtual void Show();
		virtual void Hide();
		virtual void Wait();
		virtual std::string Ask(std::string msg = "");
		virtual int GetKey();
		virtual void Render(std::string msg = "");
		virtual ~CLIView();

};

extern CLIView* debug_view;

class MainMenuCLIView : public CLIView {
	private:
		int highlight;
		std::string choices[3];
		int n_choices;
	public:
		MainMenuCLIView(int height,int width,int y,int x, int _color, bool scroll = false, Model *model = 0);
		virtual void Render(std::string msg = "");
		virtual std::string Ask(std::string msg = "");
};

class BoardCLIView : public CLIView {
	public:
	inline BoardCLIView(int height,int width,int y,int x, int _color, bool scroll = false, Model *model = 0):
									CLIView(height,width,y,x,_color,scroll, model) { }
	virtual void Render(std::string msg = "");
	void Highlight(Position position, int color);
};


#endif /* VIEW_H_ */