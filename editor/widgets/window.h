/* Plant Genererator
 * Copyright (C) 2016-2018  Floris Creyf
 *
 * Plant Genererator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plant Genererator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WINDOW_H
#define WINDOW_H

#include "editor/keymap.h"
#include "editor/qt/ui_window.h"
#include "editor/graphics/shared_resources.h"
#include "editor.h"
#include "curve_editor.h"
#include "material_editor.h"
#include "mesh_editor.h"
#include "key_editor.h"
#include "property_box.h"

class Window : public QMainWindow {
	Q_OBJECT

public:
	Window(int argc, char **argv);

public Q_SLOTS:
	void newFile();
	void openDialogBox();
	void exportDialogBox();
	void saveAsDialogBox();
	void saveDialogBox();
	void reportIssue();
	void initEditor();
	void initMeshEditor();
	void initMaterialEditor();
	void updateStatus();

private:
	Ui::Window widget;
	SharedResources shared;
	KeyMap keymap;

	Editor *editor;
	PropertyBox *propertyBox;
	CurveEditor *curveEditor;
	MaterialEditor *materialEditor;
	MeshEditor *meshEditor;
	KeyEditor *keyEditor;

	QString filename;
	QLabel *objectLabel;
	QLabel *fileLabel;
	QLabel *commandLabel;

	void keyPressEvent(QKeyEvent *event);
	void createPropertyBox();
	void createEditors();
	void setFilename(QString filename);
};

#endif /* WINDOW_H */
