/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HotSeat.class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdufaud <mdufaud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/04/30 22:47:41 by mdufaud           #+#    #+#             */
/*   Updated: 2014/05/04 22:42:53 by mdufaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HotSeat.class.hpp"
#include "MenuBlock.class.hpp"
#include "TextBlock.class.hpp"
#include "GameBlock.class.hpp"
#include "InputBlock.class.hpp"
#include "Graph.class.hpp"
#include "MainMenu.class.hpp"
#include "GameInterface.class.hpp"

HotSeat::HotSeat(Graph *graph) : AInterface(graph, GOMOKU_HOTSEAT)
{
	generateBlocks();
}

HotSeat::~HotSeat()
{
}

void		HotSeat::keyReturn(SDL_Event e)
{
	Blocklist::iterator	it = _blocks->begin();

	(void)e;
	while (it != _blocks->end())
	{
		if ((*it)->getID() == _selectedBlockID
			|| ((*it)->getID() == _hooveredBlockID
				&& _selectedBlockID < 0))
		{
			(*it)->unselect();
			_selectedBlockID = -1;
			if ((*it)->getText() == "Play")
			{
				GameInterface	*game;

				game = static_cast<GameInterface *>(_graph->getMenu(GOMOKU_GAME_INTERFACE));
				game->startGame(_playerOneInput->getInput(), _playerTwoInput->getInput(), false);
			}
			(*it)->action(0, 0);
			break ;
		}
		it++;
	}
}

bool		HotSeat::click(int x, int y)
{
	Blocklist::iterator		it = _blocks->begin();

	if (_selectedBlockID >= 0)
	{
		this->getBlockID(_selectedBlockID)->unselect();
		_selectedBlockID = -1;
	}
	while (it != _blocks->end())
	{
		if ((*it)->isInBlock(x, y))
		{
			_selectedBlockID = (*it)->selected();
			//printf("Clicked on block id: %d with name: %s\n", _selectedBlockID, (*it)->getText().c_str());
			if ((*it)->getText() == "Play")
			{
				GameInterface	*game;

				game = static_cast<GameInterface *>(_graph->getMenu(GOMOKU_GAME_INTERFACE));
				game->startGame(_playerOneInput->getInput(), _playerTwoInput->getInput(), false);
			}
			(*it)->action(x, y);
			break ;
		}
		it++;
	}
	return (false);
}

void	HotSeat::generateBlocks(void)
{
	SDL_Rect		tmpRect;
	std::string		name;
	int				h;
	int				w;
	int				y;
	int				x;

	if (_blocks)
		this->deleteBlocks();
	_blocks = new Blocklist();

	x = 10;
	y = 10;

	name = "Player Select";
	h = _graph->getH() / 5;
	w = _graph->getW() - 20;
	tmpRect = make_rect(x, y, w, h);
	ABlock	*header = new TextBlock(_graph, tmpRect, name, BACKGROUND_BMP);
	header->setFontID(HEADER_FONT);
	header->setFontStyle(TTF_STYLE_BOLD);
	_blocks->push_back(header);

	y += h + _graph->getH() / 10;
	x = _graph->getW() - (_graph->getW() / 4) * 2 - (_graph->getW() / 6) * 2;

	name = "Player 1 Name";
	h = _graph->getH() / 7;
	w = _graph->getW() / 4;
	tmpRect = make_rect(x, y, w, h);
	ABlock	*playerOneName = new TextBlock(_graph, tmpRect, name, BACKGROUND_BMP);
	_blocks->push_back(playerOneName);

	name = "Player 1 Input";
	tmpRect = make_rect(x, y + h + 20, w, h);
	ABlock	*playerOneInput = new InputBlock(_graph, tmpRect, name);
	_playerOneInput = static_cast<InputBlock *>(playerOneInput);
	_blocks->push_back(playerOneInput);

	y += 0;
	x += w + _graph->getW() / 6;

	name = "Player 2 Name";
	h = _graph->getH() / 7;
	w = _graph->getW() / 4;
	tmpRect = make_rect(x, y, w, h);
	ABlock	*playerTwoName = new TextBlock(_graph, tmpRect, name, BACKGROUND_BMP);
	_blocks->push_back(playerTwoName);

	y = y + h + 20;

	name = "Player 2 Input";
	tmpRect = make_rect(x, y, w, h);
	ABlock	*playerTwoInput = new InputBlock(_graph, tmpRect, name);
	_playerTwoInput = static_cast<InputBlock *>(playerTwoInput);
	_blocks->push_back(playerTwoInput);

	//y = _graph->getH() - (_graph->getH() / 8 * 2) - 40;
	y += h + _graph->getH() / 15;
	x = _graph->getW() / 2 - _graph->getW() / (5 * 2);

	name = "Play";
	h = _graph->getH() / 8;
	w = _graph->getW() / 5;
	tmpRect = make_rect(x, y, w, h);
	ABlock	*play = new MenuBlock(_graph, tmpRect, name, BACKGROUND_BMP, GOMOKU_GAME_INTERFACE);
	_blocks->push_back(play);

	y += h + 20;
	x = _graph->getW() / 2 - (_graph->getW() / (5 * 2)) - _graph->getW() / 10;

	name = "Return to main menu";
	h = _graph->getH() / 8;
	w = _graph->getW() / 5 + _graph->getW() / 5;
	tmpRect = make_rect(x, y, w, h);
	ABlock	*mainMenu = new MenuBlock(_graph, tmpRect, name, BACKGROUND_BMP, GOMOKU_MAIN_MENU);
	_blocks->push_back(mainMenu);

	mainMenu->setClockBlock(play, NULL, playerOneInput, NULL);
	play->setClockBlock(playerOneInput, NULL, mainMenu, NULL);
	playerOneInput->setClockBlock(mainMenu, playerTwoInput, play, NULL);
	playerTwoInput->setClockBlock(mainMenu, NULL, play, playerOneInput);

}

void	HotSeat::recalc_draw(void)
{
	generateBlocks();
}
