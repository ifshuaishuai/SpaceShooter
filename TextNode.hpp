#ifndef TEXT_NODE_HPP
#define TEXT_NODE_HPP
 
#include "SceneNode.hpp"
#include "ResourceIdentifier.hpp"

#include <SFML/Graphics.hpp>

#include <string>

class TextNode : public SceneNode
{
public:
	explicit TextNode(const FontHolder &fonts, const std::string &text);

	void setString(const std::string &text);

private:
	virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
	
private:
	sf::Text mText;
};

#endif