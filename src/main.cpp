/*
    Blubbels
    Copyright (C) 2007-2010 Christian Pulvermacher

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "game.h"
#include "main.h"

#include <QAction>
#include <QApplication>
#include <QCoreApplication>
#include <QLocale>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QToolBar>
#include <QTimer>
#include <QTranslator>



//default settings
const bool USE_ANIMATIONS = true;
const bool USE_SOUND = true;


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	gamewidget(new GameWidget(this))
{
	setWindowTitle("Blubbels");

//	gamewidget->enableAnimations(settings.value("use_animation", USE_ANIMATIONS).toBool());
//	gamewidget->enableSound(settings.value("use_sound", USE_SOUND).toBool());
	gamewidget->enableAnimations(true);
	gamewidget->enableSound(false);
	setCentralWidget(gamewidget);

	setWindowIcon(QIcon("qrc:/blubbels.png"));

	//construct actions
	QAction *new_game = new QAction(QIcon(":/icons/new.png"), tr("&New Game"), this);
	new_game->setShortcut(tr("Ctrl+N"));
	QAction *undo = new QAction(QIcon(":/icons/undo.png"), tr("&Undo"), this);
	undo->setShortcut(tr("Ctrl+Z"));
	undo->setEnabled(false);
	use_sound = new QAction(tr("&Use Sound"), this);
	use_sound->setCheckable(true);
//	use_sound->setChecked(settings.value("use_sound", USE_SOUND).toBool());
	use_animation = new QAction(tr("&Animate Bubbles"), this);
	use_animation->setCheckable(true);
	use_animation->setChecked(settings.value("use_animation", USE_ANIMATIONS).toBool());
/*
#ifndef Q_WS_MAEMO_5
	//create toolbar
	toolbar = new QToolBar(tr("Show Toolbar"), this);
	toolbar->setObjectName("toolBar"); //used by saveState()
	toolbar->addAction(new_game);
	toolbar->addSeparator();
	toolbar->addAction(undo);


	//create menu
	QMenu *game_menu = menuBar()->addMenu(tr("&Game"));
	game_menu->addAction(new_game);
	game_menu->addSeparator();
	game_menu->addAction(tr("&Statistics"), this, SLOT(showStatistics()), tr("Ctrl+S"));
	game_menu->addSeparator();
	game_menu->addAction(tr("&Quit"), this, SLOT(close()), tr("Ctrl+Q"));

	QMenu *edit_menu = menuBar()->addMenu(tr("&Edit"));
	edit_menu->addAction(undo);

	QMenu *settings_menu = menuBar()->addMenu(tr("&Settings"));
	settings_menu->addAction(use_sound);
	settings_menu->addAction(use_animation);
	settings_menu->addSeparator();
	settings_menu->addAction(toolbar->toggleViewAction());

	QMenu *help_menu = menuBar()->addMenu(tr("&Help"));
	help_menu->addAction(tr("&How to Play"), this, SLOT(help()), tr("F1"));
	help_menu->addSeparator();
	help_menu->addAction(tr("&About"), this, SLOT(about()));
	help_menu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
#else*/
	//maemo only supports a limited number of menu items
	menuBar()->addAction(tr("&Statistics"), this, SLOT(showStatistics()));
	menuBar()->addAction(tr("&About"), this, SLOT(about())); menuBar()->addAction(new_game);
	menuBar()->addAction(undo); //not visible when disabled, so we'll move it to the end to avoid confusion

	//set up portrait mode support
	/*orientation = new MaemoOrientation(this);
	connect(orientation, SIGNAL(orientationChanged(bool)),
		this, SLOT(rotate(bool)));*/
//#endif

	connect(new_game, SIGNAL(triggered()),
		gamewidget, SLOT(restart()));
	connect(undo, SIGNAL(triggered()),
		gamewidget, SLOT(undo()));
	connect(use_animation, SIGNAL(toggled(bool)),
		gamewidget, SLOT(enableAnimations(bool)));
	connect(use_sound, SIGNAL(toggled(bool)),
		gamewidget, SLOT(enableSound(bool)));

	connect(gamewidget, SIGNAL(enableUndo(bool)),
		undo, SLOT(setEnabled(bool)));

	//restoreState(settings.value("window_state", saveState()).toByteArray());
	//resize(settings.value("window_size", size()).toSize());
	//move(settings.value("window_pos", pos()).toPoint());
	setGeometry(40,0,1200,720);
	//resize(1200,720);
	//move(0,0);

	//QTimer::singleShot(6000, this, SLOT(setTheSizeCorrect()));

}


void MainWindow::about() {
	QMessageBox::about(this, tr("About Blubbels"),
		tr("<center><h1>Blubbels 1.0</h1>\
A Jawbreaker&trade; clone written in Qt 4\
<p>The original version of Blubbels can be found on <a href=\"http://sourceforge.net/projects/blubbels/\">http://sourceforge.net/projects/blubbels/</a>.</p>\
<p>The BB10 version of Blubbels can be found on <a href=\"http://github.com/Summeli/Blubbels/\">http://github.com/Summeli/Blubbels/</a>.</p>\
<small><p>&copy;2007-2010 Christian Pulvermacher &lt;pulvermacher@gmx.de&gt;<br>German Translation: &copy;2009 Dominic Hopf &lt;dh@dmaphy.de&gt;</p>\
<p>BB10 port: Antti Pohjola &lt;summeli@summeli.fi&gt;</p></small></center>\
%1")
#ifndef Q_WS_MAEMO_5
	.arg("<p>This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.</p> <p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.</p> <p>You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.</p>")
#else
	.arg("<p>This program is free software; License: <a href=\"http://www.gnu.org/licenses/gpl-2.0.html\">GNU GPL 2</a> or later.</p>")
#endif
	);
}


void MainWindow::help()
{
#ifndef Q_WS_MAEMO_5
	QMessageBox::about(this, tr("How to Play"),
		tr("<b>How to play Blubbels</b><br>\
<p>In the main window you can see lots of colored bubbles, arranged in 'stacks'.</p>\
<p>If there are two or more bubbles of the same color next to each other (either horizontally or vertically) they can be removed. You can see the points that will be awarded for doing so by hovering your mouse cursor over the bubbles.</p>\
<p>Remove the group of bubbles by clicking on them. Any bubbles that rest above these will then fall down.  After completely emptying a stack, a new stack of bubbles of random height will be moved in from the left.</p>\
<p>The game ends if there are no bubbles left on the gameboard that can be removed.</p>"));
#endif
}


//window closed, save settings
void MainWindow::closeEvent(QCloseEvent* /*ev*/)
{
	//state of use_sound is saved by GameWidget::enableSound()
	settings.setValue("use_animation", use_animation->isChecked());
	settings.setValue("window_state", saveState());
	settings.setValue("window_size", size());
	settings.setValue("window_pos", pos());
	settings.sync();

#ifdef Q_WS_MAEMO_5
	delete orientation; //stop accelerometer
#endif
}


void MainWindow::showStatistics()
{
	QMessageBox::information(this, tr("Statistics"),
	tr("<table>\
<tr><td>Average:</td><td align='right'>%1</td></tr>\
<tr><td>Highscore:</td><td align='right'>%2</td></tr>\
<tr><td>Games played:</td><td align='right'>%3</td></tr>\
</table>")
	.arg(int(settings.value("average", 0).toDouble()))
	.arg(settings.value("highscore", 0).toInt())
	.arg(settings.value("games_played", 0).toInt()));
}

int main(int argc, char **argv)
{
    // this is where the server is started etc
	QApplication app(argc, argv);

	QCoreApplication::setOrganizationName("Blubbels");
	QCoreApplication::setApplicationName("Blubbels");
/*
	QTranslator translator;
	translator.load(QString(SHARED_DIR) + "blubbels_" + QLocale::system().name());
	app.installTranslator(&translator);
*/
	MainWindow *mw = new MainWindow;
	mw->show();

	return app.exec();
    }

