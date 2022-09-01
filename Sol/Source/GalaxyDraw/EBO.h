#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>

#include "GLMacros.h"
#include "Buffer.h"

namespace GalaxyDraw 
{
	class EBO : public IndexBuffer
	{
	public:
		// ID reference of Elements Buffer Object
		GLuint ID;
		const unsigned int count;
		// Constructor that generates a Elements Buffer Object and links it to indices
		EBO(GLuint* indices, unsigned int elementCount);

		// Destructor that hanldes deleting the buffer when this class gets deleted
		virtual ~EBO();

		// Binds the EBO
		virtual void Bind() const override;
		// Unbinds the EBO
		virtual void Unbind() const override;
		// Deletes the EBO
		void Delete();

		virtual uint32_t GetCount()const override{ return (uint32_t)count; }
	};
}
#endif