#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include "GalaxyDraw/Interfaces/Buffer.h"

namespace GalaxyDraw 
{
	class OpenGL_EBO : public IndexBuffer
	{
	public:
		// ID reference of Elements Buffer Object
		uint32_t ID;
		const unsigned int count;
		// Constructor that generates a Elements Buffer Object and links it to indices
		OpenGL_EBO(uint32_t* indices, unsigned int elementCount);
		OpenGL_EBO(const uint32_t* indices, unsigned int elementCount);

		// Destructor that hanldes deleting the buffer when this class gets deleted
		virtual ~OpenGL_EBO();

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