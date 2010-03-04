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
		virtual void Render(std::string msg = "");
		virtual ~CLIView();

};


class MainMenuCLIView : public CLIView {
	public:
		inline MainMenuCLIView(int height,int width,int y,int x, int _color, bool scroll = false, Model *model = 0):
								CLIView(height,width,y,x,_color,scroll, model) { }
		virtual void Render(std::string msg = "");
};

class InfoCLIView : public CLIView {
	public:
};
class BoardCLIView : public CLIView {
	public:
	inline BoardCLIView(int height,int width,int y,int x, int _color, bool scroll = false, Model *model = 0):
									CLIView(height,width,y,x,_color,scroll, model) { }
	virtual void Render(std::string msg = "");
};
class UserCLIView : public CLIView {

};

extern CLIView* debug_view;
#endif /* VIEW_H_ */
