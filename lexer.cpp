#include <lexer.h>

#include <QColor>
#include <cstring>
#include <QDebug>

std::vector <std::vector <std::string>> KeywordData = {
	{
        "if", "else", "elif", "for", "each", "while", "endif", "end"
	},
	{
		"string", "bool", "int", "float", "void", "function", "char"
	},
	{
        "extern", "import", "require", "include", "vector", "return"
	},
	{
		"true", "false", "True", "False", "this"
	}
};

std::vector <std::pair <char, char>> EscapeCharacters = {
	std::make_pair('"', '"'),
    std::make_pair('<', '>')
};

std::vector <QColor> EscapeColors{
	QColor(232, 255, 28),
	QColor(132, 252, 3)
};

std::vector <QColor> ColorProfile{
	QColor(255,0,102),
	QColor(52, 177, 235),
	QColor(52, 235, 186),
	QColor(255, 170, 0),
};

std::string preChars = "({[]}):;#=*&- ";

lexer::lexer()
{

}

/**
 *  @todo Need to figure out a good system for multiline comments.
 *  @todo Add different syntax options depending on the language.
 */

std::vector <std::pair <std::string, QColor>> lexer::SyntaxLine(std::string line){
	std::vector <std::pair<std::string, QColor>> SyntaxData;

	bool stringMode = false;
	bool charMode = false;
	bool tagMode = false;

	bool escaped = false;
	bool escapeIndex = 0;

	std::string CurrentWord = "";
	bool foundword = false;

	for (int i = 0; i < line.size(); i++){
		foundword = false;

        /**
         *  I'm kinda mixed on this, not too sure if I completely
         *  like the way it looks or not.
         */
        if (i > 2){
            if (line[i - 1] == '.' || line.substr(i - 2, 2) == "->"){
                SyntaxData.push_back(std::make_pair(CurrentWord, QColor(255,255,255)));
                CurrentWord = "";

                int x;
                for (x = i; x < line.size(); x++){
                    if (!isalpha(line[x]) && line[x] != '_'){
                        break;
                    }
                }

                SyntaxData.push_back(std::make_pair(line.substr(i, x-i), ColorProfile[2]));
                i = x;
            }
        }

        /**
         *  An escape character is anything which contains
         *  something. For example "this is escaped".
         */
		if (!escaped){
			for (int x = 0; x < EscapeCharacters.size() && !escaped; x++){
				if (EscapeCharacters[x].first == line[i]){
					escaped = true;
					escapeIndex = x;
					CurrentWord += line[i];
					SyntaxData.push_back(std::make_pair(
						CurrentWord,
						QColor(255, 255, 255)
					));
					CurrentWord = "";
				}
			}

			if (escaped) continue;

		} else {
			if (line[i] == EscapeCharacters[escapeIndex].second){
				escaped = false;
				SyntaxData.push_back(std::make_pair(
					CurrentWord,
					EscapeColors[escapeIndex]
				));
				CurrentWord = line[i];
			} else {
				CurrentWord += line[i];
			}

			continue;
		}

        /**
         *  Pretty simple system for dealing with single
         *  line comments.
         */
		if (i + 2 <= line.size() && line.substr(i, 2) == "//"){
			std::string after = line.substr(i, line.size() - i);
			SyntaxData.push_back(std::make_pair(CurrentWord, QColor(255, 255, 255)));
			SyntaxData.push_back(std::make_pair(after, QColor(168, 168, 168)));
			return SyntaxData;
		}

		for (int y = 0; y < KeywordData.size() && !foundword; y++){
			for (int x = 0; x < KeywordData[y].size(); x++){
				bool preCharCheck = false;
				bool nexCharCheck = false;
				std::string keyword = KeywordData[y][x];

                /**
				 *  We need to check the previous and next characters
				 *  so we dont highlight a word in the middle of some
				 *  other word.
				 */
				if (i > 0 && preChars.find(line[i - 1]) != std::string::npos) preCharCheck = true;
				if (i + keyword.size() < line.size() && preChars.find(line[i + keyword.size()]) != std::string::npos) nexCharCheck = true;
				if (i == 0) preCharCheck = true;
				if (i + keyword.size() == line.size()) nexCharCheck = true;

				if (line.substr(i, keyword.size()) == keyword){
					if (!preCharCheck || !nexCharCheck){
						continue;
					}

                    /**
					 *  Push the CurrentWord value into SyntaxData as
					 *  regular coloured text.
					 */
					if (CurrentWord != ""){
						SyntaxData.push_back(std::make_pair(
							CurrentWord,
							QColor(255, 255, 255)
						));
						CurrentWord = "";
					}

                    /**
					 *  Push the keyword into SyntaxData in a pair with
					 *  the corresponding colour.
					 */
					SyntaxData.push_back(std::make_pair(
						keyword,
						ColorProfile[y]
					));

					i += keyword.size() - 1;

					// Setting foundword will break out of the keyword table search
					foundword = true;
					break;
				}
			}
		}

        /**
		 *  If no word was found just push the current character into
		 *  CurrentWord.
		 */
		if (!foundword){
			CurrentWord += line[i];
		}
	}

    /**
	 *  If any characters remain inside CurrentWord we can just push
	 *  as regular text.
	 */
	if (CurrentWord != ""){
		SyntaxData.push_back(std::make_pair(CurrentWord, QColor(255, 255, 255)));
	}

	return SyntaxData;
}
