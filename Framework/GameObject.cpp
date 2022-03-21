#include "GameObject.h"
#include "Physics.h"
#include "MMath.h"

GameObject::GameObject():
	parent(nullptr) {	
}

GameObject::GameObject(GameObject* parent_) :
	parent(parent_) {
}

GameObject::~GameObject() {}

bool GameObject::OnCreate() { return true;  }

void GameObject::OnDestroy() {}

void GameObject::Update(float deltaTime_) {
	//modelMatrix = MMath::rotate(angle, Vec3(0.0f,1.0f,0.0));
}

void GameObject::Render() const {

	Matrix4 parentModelMatrix;
	if(parent != nullptr){
		/// you are the child
		parentModelMatrix = parent->getModelMatrix();
	}
	
	Matrix3 normalMatrix = MMath::transpose(MMath::inverse((parentModelMatrix * modelMatrix)));
	//glUniformMatrix4fv(shader->getUniformID("modelMatrix"), 1, GL_FALSE, (parentModelMatrix * modelMatrix) );
	//glUniformMatrix3fv(shader->getUniformID("normalMatrix"), 1, GL_FALSE, normalMatrix);
	
	//if (texture) glBindTexture(GL_TEXTURE_2D, texture->getTextureID());

	//mesh->Render();

	//if (texture) glBindTexture(GL_TEXTURE_2D, 0);
}

void GameObject::HandleEvents(const SDL_Event &event) {} /// Just a stub