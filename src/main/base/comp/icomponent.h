#ifndef ICOMPONENT_H
#define ICOMPONENT_H

class IComponent
{
public:

	enum class Type {
		None,
		GameObject,
		Transformation,
		Renderer,
		Geometry,
		Material,
		Shader,
		Texture,
		GeometryRenderer,
		Script
	};

	explicit IComponent(Type type);
	virtual ~IComponent() = default;

	Type getType();

public:
	virtual void init() = 0;
	virtual void update() = 0;

private:
	Type mType = Type::None;
};

#endif // ICOMPONENT_H