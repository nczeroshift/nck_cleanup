#include "CharacterBounds.h"

#include <vector>
#include <algorithm>
#include <map>

#include "nckUtils.h"
#include "nckLog.h"
#include "nckMathUtils.h"
#include "nckStringUtils.h"

static char chars_codes[] = 
"ABCDEFGHIJKLM"
"NOPQRSTUVWXY"
"Z??????????abc"
"defghijklmnopqr"
"stuvwxyz??????"
"????????????????"
"??1234567890?!.,;"
":-&[]+*(){}/\\\"'|="
"_#$<>@?Ǫ???~^`?";

bool sort_characters_by_x(const CharacterBounds & a, const CharacterBounds & b) {
	return a.GetMinX() < b.GetMinX();
}

void Algorithm_DetectRows(std::list<CharacterBounds> * characters, std::vector<int> * lineHeights) {
	lineHeights->clear();

	int min_height = -1;
	int max_height = -1;

	std::vector<int> heights;
	std::vector<int> bottoms;

	ListFor(CharacterBounds, (*characters), i) {
		bottoms.push_back(i->GetMaxY());
		int height = i->GetMaxY() - i->GetMinY();
		heights.push_back(height);
		if (min_height == -1 || height < min_height) min_height = height;
		if (max_height == -1 || height > max_height) max_height = height;
	}

	std::sort(bottoms.begin(), bottoms.end());
	std::sort(heights.begin(), heights.end());

	int p50_height = heights[heights.size() / 2];

	int last_y = 0;
	std::vector<int> tmp_values;
	for (int i = 0; i < bottoms.size(); i++) {
		int v = bottoms[i];
		tmp_values.push_back(v);
		std::sort(tmp_values.begin(), tmp_values.end());

		int p50_v = tmp_values[tmp_values.size() >> 1];

		if (v - p50_v > p50_height) {
			lineHeights->push_back(p50_v);
			tmp_values.clear();
		}

		/*if (bottoms[i] - last_y > p50_height) {
		Core::Log::Debug("last line: " + Core::StringWithInt(last_y) + ", new line at: " + Core::StringWithInt(bottoms[i]) + "\n");
		if ((lineHeights.size() == 0 || lineHeights[lineHeights.size() - 1] < last_y) && last_y > 0)
		lineHeights.push_back(last_y);
		if ((lineHeights.size() == 0 || lineHeights[lineHeights.size() - 1] < bottoms[i])&& bottoms[i] > 0)
		lineHeights.push_back(bottoms[i]);
		last_y = bottoms[i];

		tmp_values.clear();
		}*/
	}

	lineHeights->push_back(tmp_values[tmp_values.size() >> 1]);

	/*int count = lineHeights.size();
	lineHeights.clear();

	for (int i = 0; i < count; i++) {
		lineHeights.push_back(22 + 28 * i);
	}*/

	for (int i = 0; i < lineHeights->size(); i++) {
		Core::Log::Debug("Line (" + Core::StringWithInt(i)  + ") = " + Core::StringWithInt((*lineHeights)[i])+ " alt = " + Core::StringWithInt(22 + 28 * i)+"\n");
	}
}

void Algorithm_SortAndFix(std::list<CharacterBounds> * characters, std::vector<int> lineHeights) {
	const int y_margin = 12;

	std::map<int, std::list<CharacterBounds>> tmpMap;

	for (int i = 0; i < lineHeights.size(); i++) {
		tmpMap.insert(std::pair<int, std::list<CharacterBounds>>(lineHeights[i], std::list<CharacterBounds>()));
	}

	ListFor(CharacterBounds, (*characters), i) {
		int min_y = i->GetMinY();
		int max_y = i->GetMaxY();

        bool didAdd = false;

		int last_y = 0;
		for (int j = 0; j < lineHeights.size(); j++) {
			int line_y = lineHeights[j];

			if (min_y - y_margin < line_y && max_y + y_margin > line_y) {
				i->SetLineHeight(line_y);
				(tmpMap.find(line_y)->second).push_back(*i);
                didAdd = true;
				break;
			}

			if (line_y > max_y) {
				i->SetLineHeight(line_y);
				(tmpMap.find(line_y)->second).push_back(*i);
                didAdd = true;
				break;
			}

            if (min_y - line_y  < y_margin) {
                i->SetLineHeight(line_y);
                (tmpMap.find(line_y)->second).push_back(*i);
                didAdd = true;
                break;
            }

			last_y = line_y;
		}

        if (!didAdd) {
            Core::Log::Debug("!");
        }

	}

	std::list<CharacterBounds> replace;

	for (int i = 0; i < lineHeights.size(); i++) {
		std::list<CharacterBounds> tmpList = tmpMap.find(lineHeights[i])->second;
		tmpList.sort(sort_characters_by_x);
		replace.insert(replace.end(), tmpList.begin(), tmpList.end());
	}

	int count = 0;
	ListFor(CharacterBounds, replace, i) {
		if (count >= sizeof(chars_codes)){
			break;
        }

	    i->SetCode(chars_codes[count]);
		count++;
	}

	*characters = replace;
}

int Algorithm_GetMaxCharactersWidth(std::list<CharacterBounds> * characters) {

	int max_width = 0;
	ListFor(CharacterBounds, (*characters), i) {
		max_width = MAX(max_width, i->GetMaxX() - i->GetMinX());
	}

	/*ListFor(CharacterBounds, (*characters), i) {
		int width = i->GetMaxX() - i->GetMinX();

		int diff = max_width - width;

		int left_margin = diff / 2;
		int right_margin = diff - left_margin;

		i->SetMargin(left_margin, right_margin);
	}*/

	//Core::Log::Debug("Max char width = " + Core::StringWithInt(max_width)+"\n");

	return max_width;
}