#ifndef BUILDABLE_H
#define BUILDABLE_H

/// <summary>
/// Creates both a variable declaration and a setter method for the variable in question.
/// Does not work with reference types or pointers though.
/// </summary>
/// <typeparam name="BuildableType"> The builder type for a given class</typeparam>
/// <typeparam name="PropertyType"> The type for the particular property you want to generate a method for</typeparam>
/// <typeparam name="propertyName"> The variable name and method name for the property.</typeparam>
#define BUILDABLE_PROPERTY(BuildableType, PropertyType, propertyName)\
PropertyType propertyName;\
\
BuildableType* With_##propertyName(PropertyType value)\
{\
    this->propertyName = value;\
    return this;\
}

template<typename T>
class IBuildable
{
public:
	virtual T Build() = 0;

};

#endif