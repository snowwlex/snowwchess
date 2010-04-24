/*
 * view.h
 *
 *
 *      Author: snowwlex
 */

#ifndef VIEW_H_
#define VIEW_H_

class Model;

class CLIView {
	private:
		Model *myModel;
	protected:
		WINDOW *myWindow;
		int myColor;
	public:
		CLIView(int height,int width,int y,int x, int _color, bool scroll = false, Model *model = 0);
		virtual void show();
		virtual void hide();
		virtual void wait();
		virtual void clear();
		virtual std::string ask(std::string msg = "");
		virtual int getKey();
		virtual void render(std::string msg = "");
		virtual ~CLIView();

};

extern CLIView* debugView;


class MainMenuCLIView : public CLIView {
	private:
		int myHighlight;
		std::string myChoices[3];
		int nChoices;
	public:
		MainMenuCLIView(int height,int width,int y,int x, int _color, bool scroll = false, Model *model = 0);
		virtual void render(std::string msg = "");
		virtual std::string ask(std::string msg = "");
};

class BoardCLIView : public CLIView {
	public:
	inline BoardCLIView(int height,int width,int y,int x, int _color, bool scroll = false, Model *model = 0):
									CLIView(height,width,y,x,_color,scroll, model) { }
	virtual void render(std::string msg = "");
	void highlight(Position position, int color);
};


#endif /* VIEW_H_ */
