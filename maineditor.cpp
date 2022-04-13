#include <lexer.h>
#include <maineditor.h>

#include <QPainter>
#include <QFont>
#include <QPoint>
#include <QEvent>
#include <QPainterPath>
#include <QDebug>

MainEditor::MainEditor(QWidget *parent) : QWidget(parent)
{
	this->parent = parent;
	this->repaint();

	this->cursorx = 0;
	this->cursory = 0;
	this->cursors = 0;
	this->scroll = 0;

	this->fontName = "Graph 35+ pix";  // Fontname, this should be a setting at some point
	this->fontSize = DF_FONT_SIZE;     // Pretty simple, this is the font size
	QFont font(this->fontName.c_str(), this->fontSize);
	QFontMetrics fm(font);
	this->fontHeight = fm.height();    // Keep a copy so we don't have to keep recalculating
	this->linespace = 5;               // This is the space inbetween lines

	this->lexerClass = lexer();

	File defaultFile;
	defaultFile.filename = "New file";
	defaultFile.newfile = true;
	defaultFile.FileData = {
		"// This is a blank file,",
		"// press Ctrl + O and get some work done!"
	};

	for (int i = 0; i < defaultFile.FileData.size(); i++){
		defaultFile.SyntaxData.push_back(lexerClass.SyntaxLine(defaultFile.FileData[i]));
	}

	this->files.push_back(defaultFile);
	this->fi = 0;                      // File index, short name to make long lines shorter
}

MainEditor::~MainEditor()
{

}

void MainEditor::mouseClickUpdate(QMouseEvent *e){
	int ry = e->pos().ry();
	int rx = e->pos().rx();

	for (int i = mouseCursorLookup.size() - 1; i > 0; i--){
		if (ry >= mouseCursorLookup[i].drawY){
			QFont font(this->fontName.c_str(), this->fontSize);
			QFontMetrics fm(font);

			/**
			 *  Take the relative X of the coords and devide it by the size
			 *  of a single character to get where the cursor should be on the 
			 *  X axis.
			 *
			 *  @note floor / ceil can make this somewhat inaccurate so I will
			 *        have to work on that at some point.
			 */
			int singleWidth = fm.horizontalAdvance("A");
			int cursorPosDiv = (rx - 20) / singleWidth;
			cursorPosDiv = floor(cursorPosDiv);

			/**
			 *  If the cursor X is less than the actual text then just set it to
			 *  0, this will pretect again seg faults and compensate for the 
			 *  inaccuracy.
			 */
			if (rx < 20) cursorPosDiv = 0;

			this->cursory = mouseCursorLookup[i].cursorY;
			this->cursorx = cursorPosDiv;

			/**
			 *  If the relative X is further than the size of the current line
			 *  just set the cursor to be at the size of the line.
			 */
			if (rx - 20 > singleWidth * files[fi].FileData[cursory].size()){
				this->cursorx = files[fi].FileData[cursory].size();
			}

			this->cursors = this->cursorx;
			this->repaint();
			break;
		}
	}
}

void MainEditor::updateTitle(){
	std::string filepath = files[fi].filename;
	std::string shortFilename = filepath.substr(filepath.find_last_of("/\\") + 1);

	std::string titleTemplate = "Very tiny text editor :: %X%N (%Y/%Z)";

	titleTemplate.replace(titleTemplate.find("%N"), 2, files[fi].edited == true ? "*" : "");
	titleTemplate.replace(titleTemplate.find("%X"), 2, shortFilename);
	titleTemplate.replace(titleTemplate.find("%Y"), 2, std::to_string(fi + 1));
	titleTemplate.replace(titleTemplate.find("%Z"), 2, std::to_string(files.size()));

	parent->setWindowTitle(QString::fromStdString(titleTemplate));
}

void MainEditor::paintEvent(QPaintEvent *e){
	QWidget::paintEvent(e);

	this->mouseCursorLookup.clear();

	QPainter painter(this);
	QFont font(this->fontName.c_str(), this->fontSize);
	QFontMetrics fm(font);
	painter.setFont(font);

	/**
	 *  Draw the main text, all nice and colored too!
	 */
	int dx = 0;
	int dy = scroll;
	int drawn = 0;

	for (int y = 0; y < files[fi].SyntaxData.size(); y++){
		for (int x = 0; x < files[fi].SyntaxData[y].size(); x++){
			painter.setPen(files[fi].SyntaxData[y][x].second);
			painter.drawText(10 + dx, 20 + dy + (linespace * y), files[fi].SyntaxData[y][x].first.c_str());

			MouseLookupItem item;
			item.drawY = (20 + dy + (linespace * y));
			item.cursorY = y;
			mouseCursorLookup.push_back(item);

			dx += fm.horizontalAdvance(files[fi].SyntaxData[y][x].first.c_str());
		}

		dx = 0;
		dy += fm.height();
	}

	/**
	 *  Draw cursor
	 */
	int cx = fm.horizontalAdvance(files[fi].FileData[cursory].substr(0, cursorx).c_str());
	int cy = fontHeight * (cursory + 1) - 1 + (linespace * cursory) + scroll;

	painter.setPen(Qt::white);
	painter.drawLine(10 + cx, 5 + cy, 10 + cx, 5 + cy + fm.height() + linespace / 2);
}
