//-------------------------------------------------------------------------
//
// The MIT License (MIT)
//
// Copyright (c) 2022 Andrew Duncan
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------

#include <algorithm>

#include "picosystem.hpp"
#include "pico/rand.h"

#include "images.h"
#include "puzzle.h"

//-------------------------------------------------------------------------

using namespace picosystem;

//-------------------------------------------------------------------------

Puzzle::Puzzle()
:
    m_board
    {
        0x01, 0x02, 0x03, 0x04,
        0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C,
        0x0D, 0x0E, 0x0F, 0x00
    },
    m_tileBuffers(
        { {
            { tileWidth, tileHeight, piece0, false },
            { tileWidth, tileHeight, piece1, false },
            { tileWidth, tileHeight, piece2, false },
            { tileWidth, tileHeight, piece3, false },
            { tileWidth, tileHeight, piece4, false },
            { tileWidth, tileHeight, piece5, false },
            { tileWidth, tileHeight, piece6, false },
            { tileWidth, tileHeight, piece7, false },
            { tileWidth, tileHeight, piece8, false },
            { tileWidth, tileHeight, piece9, false },
            { tileWidth, tileHeight, piece10, false },
            { tileWidth, tileHeight, piece11, false },
            { tileWidth, tileHeight, piece12, false },
            { tileWidth, tileHeight, piece13, false },
            { tileWidth, tileHeight, piece14, false },
            { tileWidth, tileHeight, piece15, false },
        } }),
    m_blankLocation{ 3, 3 }
{
}

//-------------------------------------------------------------------------

int
Puzzle::getInversionCount() const
{
    int inversions = 0;

    for (int i = 0 ; i < boardSize - 1 ; ++i)
    {
        for (int j = i + 1 ; j < boardSize ; ++j)
        {
            if (m_board[i] && m_board[j] && (m_board[i] > m_board[j]))
            {
                ++inversions;
            }
        }
    }

    return inversions;
}

//-------------------------------------------------------------------------

bool
Puzzle::isSolvable() const
{
    const auto inversions = getInversionCount();
    bool solvable = inversions & 1;

    if (m_blankLocation.y & 1)
    {
        solvable = not solvable;
    }

    return solvable;
}

//-------------------------------------------------------------------------

bool
Puzzle::isSolved() const
{
    bool solved = true;

    for (int i = 0 ; i < boardSize - 1 ; ++i)
    {
        if (m_board[i] != (i + 1))
        {
            solved = false;
        }
    }

    return solved;
}

//-------------------------------------------------------------------------

void
Puzzle::init()
{
    do
    {
        for (int i = 0 ; i < boardSize - 1 ; ++i)
        {
            auto j = i + get_rand_32() / (UINT32_MAX / (boardSize - i) + 1);
            std::swap(m_board[i], m_board[j]);
        }

        for (int i = 0 ; i < boardSize ; ++i)
        {
            if (m_board[i] == 0)
            {
                m_blankLocation.x = i % puzzleWidth;
                m_blankLocation.y = i / puzzleWidth;
                break;
            }
        }
    }
    while (not isSolvable() or isSolved());
}

//-------------------------------------------------------------------------

void
Puzzle::update(uint32_t tick)
{
    if (pressed(A))
    {
        init();
    }
    else
    {
        int dx = 0;
        int dy = 0;

        if (pressed(UP))
        {
            dy = 1;
        }
        else if (pressed(DOWN))
        {
            dy = -1;
        }
        else if (pressed(LEFT))
        {
            dx = 1;
        }
        else if (pressed(RIGHT))
        {
            dx = -1;
        }

        if ((dx != 0) or (dy != 0))
        {
            const Location newLocation = {.x = m_blankLocation.x + dx,
                                          .y = m_blankLocation.y + dy};

            if ((newLocation.x >= 0) &&
                (newLocation.x < puzzleWidth) &&
                (newLocation.y >= 0) &&
                (newLocation.y < puzzleHeight))
            {
                const auto indexNew = newLocation.x + (newLocation.y * puzzleWidth);
                const auto indexBlank = m_blankLocation.x + (m_blankLocation.y * puzzleWidth);
                std::swap(m_board[indexNew], m_board[indexBlank]);

                m_blankLocation = newLocation;
            }
        }
    }

    if (isSolved())
    {
        led(0, 50, 0);
    }
    else
    {
        led(0, 0, 0);
    }
}

//-------------------------------------------------------------------------

void
Puzzle::draw(uint32_t tick)
{
    for (int j = 0 ; j < puzzleHeight ; ++j)
    {
        for (int i = 0 ; i < puzzleWidth ; ++i)
        {
           const auto tile = m_board[i + (j * puzzleWidth)];
           blit(&(m_tileBuffers[tile]),
                0,
                0,
                tileWidth,
                tileHeight,
                i * tileWidth,
                j * tileHeight);
        }
    }
}
