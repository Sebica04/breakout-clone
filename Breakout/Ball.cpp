#include "Ball.hpp"
#include "iostream"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~constructor~~~~~~~~~~~~~~~~~~~~~~~~~~//

Ball::Ball(float startX, float startY) {
	setPosition({ startX, startY });

	if (!m_ballTexture.loadFromFile("assets/images/ball.png")) {
		std::cerr << "Failed to load the ball texture" << std::endl;
	}

	m_shape.setRadius(12.0f);

	m_shape.setTexture(&m_ballTexture);

	m_shape.setOrigin(m_shape.getGeometricCenter());

	m_shape.setTextureRect(sf::IntRect({0,0},{24, 24}));

	m_isGlowing = false;

	m_speed = 3.0f;
	m_velocity = normalize({ -1.f, -1.f }) * m_speed;
	m_isHeld = true;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


//~~~~~~~~~~~~~~~~~~~~~~~~~~helper functions~~~~~~~~~~~~~~~~~~~~~~~~//

sf::Vector2f Ball::getPosition() const { return sf::Transformable::getPosition(); }

sf::FloatRect Ball::getGlobalBounds() const { return getTransform().transformRect(m_shape.getGlobalBounds()); }

void Ball::glow(){
	m_isGlowing = true;
	m_shape.setTextureRect(sf::IntRect({ 24, 0 }, { 24, 24 }));
	m_glowClock.restart();
}

sf::Vector2f Ball::normalize(const sf::Vector2f& source) {

	//calculates the lenght of the vector
	float length = std::sqrt((source.x * source.x) + (source.y * source.y));

	if (length != 0)
		//make the vector lenght equal to 1
		return sf::Vector2f(source.x / length, source.y / length);
	else
		return source;
}

void Ball::hold(const Paddle& paddle)
{
	m_isHeld = true;
	setPosition({ paddle.getPosition().x, paddle.getPosition().y - 30.f });
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


//~~~~~~~~~~~~~~~~~~~everything about the ballmovement~~~~~~~~~//

void Ball::update(Paddle& paddle, std::vector<Brick>& bricks, int& score, int& lives) {
	
	if (m_isHeld){

		setPosition({ paddle.getPosition().x, paddle.getPosition().y - 22.f });

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		{
			m_isHeld = false;
			m_velocity = normalize({ -0.5f, -1.0f }) * m_speed;
		}

		return;
	}

	move(m_velocity);
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~screenbounds~~~~~~~~~~~~~//

	if (getPosition().x < 16.0f) {
		setPosition({ 16.0f, getPosition().y });
		m_velocity.x = -m_velocity.x;
		this->glow();
	}	

	else if (getPosition().x > 960.0f - 16.0f) {
			setPosition({ 960.0f - 16.0f, getPosition().y });
			m_velocity.x = -m_velocity.x;
			this->glow();
	}

	if (getPosition().y < 16.0f) {
		setPosition({ getPosition().x, 16.0f });
		m_velocity.y = -m_velocity.y;
		this->glow();
	}

	else if (getPosition().y > 800.0f + 120.0f) {
		lives -= 1;
		hold(paddle);
	}

	//~~~~~~~~~~~~~~~~intersection with the paddle~~~~~~~~~~~~//

	sf::FloatRect ballBounds = getGlobalBounds();
	sf::FloatRect paddleBounds = paddle.getGlobalBounds();

	if (ballBounds.findIntersection(paddleBounds)) {

		//calculate where on the paddle did the ball hit
		float intersectX = getPosition().x - paddle.getPosition().x;

		//normalize the result in a (-1, 1) range
		float normalizedIntersect = intersectX / (paddle.getSize().x / 2.f);

		//create a vector that has the correct angle but has weird speed
		sf::Vector2f bounceDirection(normalizedIntersect, -1.0f);		//since the x is between (-1, 1) we can t have a greater angle than 45 degrees
																		//can increase the angle by amplyfing the x with a bounce factor or reduce the y
																		
		//normalize the vector so that we keed its direction but adjust the speed
		m_velocity = normalize(bounceDirection) * m_speed;

		this->glow();

		paddle.onBallHit();

	}

	//~~~~~~~~~~~intersection with the bricks~~~~~~~~~~~~//

	for (auto& brick : bricks) {
		if (brick.isDestroyed()) {
			continue;
		}

		sf::FloatRect ballBounds = this->getGlobalBounds();
		sf::FloatRect brickBounds = brick.getGlobalBounds();

		if (auto intersectionRect = ballBounds.findIntersection(brickBounds)) {
			
			brick.takeDamage();
			if (brick.health() == 0) {
				brick.destroy();
				score += 100;
			}
			this->glow();


			if (intersectionRect->size.x < intersectionRect->size.y) {
				
				if (ballBounds.position.x < brickBounds.position.x) {
					this->move({ -intersectionRect->size.x, 0.f });
				}
				else {
					this->move({ intersectionRect->size.x, 0.f });
				}
				m_velocity.x = -m_velocity.x;
			}
			else
			{
				if (ballBounds.position.y < brickBounds.position.y) {
					this->move({ 0.f, -intersectionRect->size.y });
				}
				else {
					this->move({ 0.f, intersectionRect->size.y });
				}
				m_velocity.y = -m_velocity.y;
			}
			break;
		}
	}

	//~~~~~~~~ball roatations and glow stop ~~~~~~~~~~~~~~~//

	this->rotate(sf::degrees(m_velocity.x * 2));

	if (m_isGlowing && m_glowClock.getElapsedTime().asMilliseconds() > 150)
	{
		m_isGlowing = false;

		m_shape.setTextureRect(sf::IntRect({ 0, 0 }, { 24, 24 }));
	}
	
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(m_shape, states);
}