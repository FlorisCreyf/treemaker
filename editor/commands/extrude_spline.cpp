/* Plant Genererator
 * Copyright (C) 2018  Floris Creyf
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

#include "extrude_spline.h"
#include "remove_spline.h"
#include <iterator>

ExtrudeSpline::ExtrudeSpline(
	PointSelection *selection, pg::Spline *spline, TranslationAxes *axes,
	const Camera *camera) :
	prevSelection(*selection), newSelection(*selection),
	moveSpline(selection, spline, axes, camera)
{
	this->selection = selection;
	this->spline = spline;
}

void ExtrudeSpline::setClickOffset(int x, int y)
{
	moveSpline.setClickOffset(x, y);
}

void ExtrudeSpline::execute()
{
	std::set<int> points = selection->getPoints();
	std::set<int> newPoints;
	int o = 0;

	for (auto it = points.begin(); it != points.end(); ++it) {
		int p = *it;
		p = spline->insert(p + o, spline->getControls()[p + o]);
		newPoints.insert(p);
		o += spline->getDegree();
	}

	selection->setPoints(newPoints);
}

void ExtrudeSpline::undo()
{
	newSelection = *selection;
	prevSpline = *spline;
	RemoveSpline remove(selection, spline);
	remove.execute();
	*selection = prevSelection;
}

void ExtrudeSpline::redo() {
	*spline = prevSpline;
	*selection = newSelection;
}

bool ExtrudeSpline::onMouseMove(QMouseEvent *event)
{
	bool update = moveSpline.onMouseMove(event);
	this->done = moveSpline.isDone();
	return update;
}

bool ExtrudeSpline::onMousePress(QMouseEvent *event)
{
	bool update = moveSpline.onMousePress(event);
	this->done = moveSpline.isDone();
	return update;
}

bool ExtrudeSpline::onMouseRelease(QMouseEvent *event)
{
	bool update = moveSpline.onMouseRelease(event);
	this->done = moveSpline.isDone();
	return update;
}
