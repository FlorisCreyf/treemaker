/* Copyright 2017 Floris Creyf
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "stem.h"
#include <limits>
#include <cmath>

using namespace pg;

Stem::Stem(Stem *parent)
{
	init(parent);
}

Stem::~Stem()
{

}

Stem::Stem(const Stem &original)
{
	copy(original);
}

void Stem::init(Stem *parent)
{
	this->joints.clear();
	this->leaves.clear();
	this->swelling = Vec2(1.5, 3.0);
	this->material[0] = 0;
	this->material[1] = 0;
	this->resolution = 10;
	this->nextSibling = nullptr;
	this->prevSibling = nullptr;
	this->child = nullptr;
	this->parent = parent;
	if (parent == nullptr) {
		this->depth = 0;
		this->position = 0.0f;
		this->location = Vec3(0.0f, 0.0f, 0.0f);
	} else
		this->depth = parent->depth + 1;
}

Stem &Stem::operator=(const Stem &stem)
{
	copy(stem);
	return *this;
}

void Stem::copy(const Stem &stem)
{
	this->nextSibling = stem.nextAvailable;
	this->prevSibling = stem.prevSibling;
	this->parent = stem.parent;
	this->child = stem.child;
	this->depth = stem.depth;
	this->path = stem.path;
	this->resolution = stem.resolution;
	this->position = stem.position;
	this->location = stem.location;
	this->material[0] = stem.material[0];
	this->material[1] = stem.material[1];
	this->leaves = stem.leaves;
	this->swelling = stem.swelling;
}

bool Stem::operator==(const Stem &stem) const
{
	return (
		this->nextSibling == stem.nextSibling &&
		this->prevSibling == stem.prevSibling &&
		this->child == stem.child &&
		this->parent == stem.parent &&
		this->depth == stem.depth &&
		this->path == stem.path &&
		this->resolution == stem.resolution &&
		this->position == stem.position &&
		this->location == stem.location &&
		this->material[0] == stem.material[0] &&
		this->material[1] == stem.material[1] &&
		this->swelling == stem.swelling &&
		this->leaves == stem.leaves
	);
}

bool Stem::operator!=(const Stem &stem) const
{
	return !(*this == stem);
}

size_t Stem::addLeaf(const Leaf &leaf)
{
	this->leaves.push_back(leaf);
	return this->leaves.size() - 1;
}

void Stem::insertLeaf(const Leaf &leaf, size_t index)
{
	this->leaves.insert(this->leaves.begin() + index, leaf);
}

size_t Stem::getLeafCount() const
{
	return this->leaves.size();
}

Leaf *Stem::getLeaf(size_t index)
{
	return &this->leaves.at(index);
}

const Leaf *Stem::getLeaf(size_t index) const
{
	return &this->leaves.at(index);
}

const std::vector<Leaf> &Stem::getLeaves() const
{
	return this->leaves;
}

void Stem::removeLeaf(size_t index)
{
	this->leaves.erase(this->leaves.begin() + index);
}

void Stem::setResolution(int resolution)
{
	this->resolution = resolution;
}

int Stem::getResolution() const
{
	return this->resolution;
}

void Stem::setPath(Path &path)
{
	this->path = path;
	if (this->parent)
		this->path.generate(true);
	else
		this->path.generate(false);
	updatePositions(this);
}

Path Stem::getPath() const
{
	return this->path;
}

void Stem::updatePositions(Stem *stem)
{
	Stem *child = stem->child;
	while (child != nullptr) {
		child->setPosition(child->position);
		updatePositions(child);
		child = child->nextSibling;
	}
}

void Stem::setPosition(float position)
{
	if (this->parent != nullptr) {
		Path parentPath = this->parent->getPath();
		Vec3 point = parentPath.getIntermediate(position);
		if (std::isnan(point.x))
			this->location = point;
		else
			this->location = parent->getLocation() + point;
		this->position = position;
		updatePositions(this);
	}
}

float Stem::getPosition() const
{
	return this->position;
}

Vec3 Stem::getLocation() const
{
	return this->location;
}

void Stem::setMaterial(int feature, long material)
{
	this->material[feature] = material;
}

long Stem::getMaterial(int feature) const
{
	return this->material[feature];
}

Stem *Stem::getParent()
{
	return this->parent;
}

const Stem *Stem::getParent() const
{
	return this->parent;
}

Stem *Stem::getSibling()
{
	return this->nextSibling;
}

const Stem *Stem::getSibling() const
{
	return this->nextSibling;
}

Stem *Stem::getChild()
{
	return this->child;
}

const Stem *Stem::getChild() const
{
	return this->child;
}

bool Stem::isDescendantOf(Stem *stem) const
{
	const Stem *descendant = this;
	if (this->depth > stem->getDepth()) {
		while (descendant != nullptr) {
			if (stem == descendant->parent)
				return true;
			descendant = descendant->parent;
		}
	}
	return false;
}

int Stem::getDepth() const
{
	return this->depth;
}

void Stem::setSwelling(Vec2 scale)
{
	this->swelling = scale;
}

Vec2 Stem::getSwelling() const
{
	return this->swelling;
}

Vec2 Stem::getLimitedSwelling(float limit) const
{
	if (!this->parent)
		return this->swelling;
	Vec2 swelling = this->swelling;
	Path path = this->parent->getPath();
	float parentRadius = path.getIntermediateRadius(this->position);
	float radius = this->path.getMaxRadius();
	float ratio = parentRadius / radius;
	if (ratio < limit) {
		swelling.x = 1.0f;
		swelling.y = 1.0f;
	} else if (this->swelling.x > ratio) {
		swelling.x = ratio;
		swelling.y /= ratio;
		if (swelling.y < 1.0f)
			swelling.y = 1.0f;
	}
	return swelling;
}

std::vector<Joint> Stem::getJoints() const
{
	return this->joints;
}

bool Stem::hasJoints() const
{
	return !this->joints.empty();
}

void Stem::addJoint(Joint joint)
{
	joint.updateLocation(path.get(joint.getPathIndex()));
	this->joints.push_back(joint);
}

void Stem::clearJoints()
{
	this->joints.clear();
	Stem *child = this->child;
	while (child) {
		child->clearJoints();
		child = child->nextSibling;
	}
}
