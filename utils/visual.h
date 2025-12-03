#pragma once

struct visual_t
{
    enum pos
    {
        left = 0,
        top = 1,
        right = 2,
        bottom = 3
    } pos = left;
public:
    bool enabled = false;
    bool outlined = true;
    bool team_col = false;
};