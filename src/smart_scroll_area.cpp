/**
 * This file is a part of Qtpfsgui package.
 * ---------------------------------------------------------------------- 
 * Copyright (C) 2007 Giuseppe Rota
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * ---------------------------------------------------------------------- 
 *
 * @author Giuseppe Rota <grota@users.sourceforge.net>
 */

#include "smart_scroll_area.h"

SmartScrollArea::SmartScrollArea( QWidget *parent, QLabel *imagelabel ) : QScrollArea(parent), imageLabel(imagelabel), scaleFactor(1.0), fittingwin(false) {
	setBackgroundRole(QPalette::Light);
	imageLabel->setBackgroundRole(QPalette::Base);
	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	imageLabel->setScaledContents(true);
	setWidgetResizable(false);
	setWidget(imageLabel);
}

void SmartScrollArea::zoomIn() {
	scaleImage(1.25);
}
void SmartScrollArea::zoomOut() {
	scaleImage(0.8);
}
void SmartScrollArea::fitToWindow(bool checked) {
	fittingwin=checked;
	if (checked)
		scaleLabelToFit();
	else
		// restore to the previous zoom factor
		scaleImage(1);
}
void SmartScrollArea::normalSize() {
	imageLabel->adjustSize();
	scaleFactor = 1.0;
}
void SmartScrollArea::scaleLabelToFit() {
	int sa_width=this->size().width();
	int sa_height=this->size().height();
	float imageratio=float(imageLabel->pixmap()->size().width())/float(imageLabel->pixmap()->size().height());
	float factor=1;
	if (sa_width<imageratio*sa_height) {
		factor=float(sa_width)/float(imageLabel->pixmap()->size().width());
	} else {
		factor=float(sa_height)/float(imageLabel->pixmap()->size().height());
	}
	imageLabel->resize(factor * 0.99 * imageLabel->pixmap()->size());
}
void SmartScrollArea::scaleImage(double factor) {
	scaleFactor *= factor;
	imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());
	adjustScrollBar(horizontalScrollBar(), factor);
	adjustScrollBar(verticalScrollBar(), factor);
}
void SmartScrollArea::adjustScrollBar(QScrollBar *scrollBar, double factor) {
	scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
}
void SmartScrollArea::resizeEvent ( QResizeEvent * e) {
	if (fittingwin) {
		scaleLabelToFit();
	} else {
		//this seems to be a bug in qt4
		imageLabel->resize(imageLabel->size()-QSize(0,1));
		imageLabel->resize(imageLabel->size()+QSize(0,1));
		
// 		QScrollBar *h=horizontalScrollBar();
// 		QScrollBar *v=verticalScrollBar();
// 		int fixwidth=imageLabel->size().width()+v->size().width()-this->size().width();
// 		if (fixwidth>0)
// 			h->setMaximum(qMax(0,fixwidth));
// 		else
// 			h->setMaximum(0);
// 		int fixheight=imageLabel->size().height()-this->size().height();
// 		if (fixheight>0)
// 			v->setMaximum(qMax(0,fixheight));
// 		else
// 			v->setMaximum(0);
	}
}

void SmartScrollArea::mouseMoveEvent(QMouseEvent *e) {
	if (e->buttons()==Qt::MidButton) {
		QPoint diff = e->pos() - mousePos;
		if (e->modifiers()==Qt::ShiftModifier)
			diff*=5;
		mousePos = e->pos();
		verticalScrollBar()->setValue(verticalScrollBar()->value() + 
	diff.y());
		horizontalScrollBar()->setValue(horizontalScrollBar()->value() + 
	diff.x());
	}
}
