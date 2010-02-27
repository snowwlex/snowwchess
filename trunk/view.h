/*
 * view.h
 *
 *
 *      Author: snowwlex
 */

#ifndef VIEW_H_
#define VIEW_H_

class View {
	protected:
		Model *myModel;
		int myColor;
	public:
		WINDOW *myWindow;
		View(int x1,int x2,int y1,int y2, int _color, bool scroll = false, Model *model = 0);
		virtual std::string Ask(std::string msg = "");
		virtual void Render(std::string msg = "");
		virtual ~View();

};


class MainMenuCLIView : public View {
	public:
		MainMenuCLIView(int x1,int x2,int y1,int y2, int _color, bool scroll = false, Model *model = 0);
		virtual void Render(std::string msg = "");
};

class InfoCLIView : public View {
	public:
};
class BoardCLIView : public View {
	public:
	inline BoardCLIView(int x1,int x2,int y1,int y2, int _color, bool scroll = false, Model *model = 0): View(x1,y1,x2,y2,_color,scroll, model) { }
	virtual void Render(std::string msg = "");
};
class UserCLIView : public View {

};
#endif /* VIEW_H_ */
