#include<ctime>
#include<string>
#include<vector>
#include<unordered_map>
#include<SFML/Graphics.hpp>
#include"trie.h"
#include"hash.h"

int windowState = 0;
bool hide = true;
sf::Sprite* bg = nullptr;
std::unordered_map<std::string, sf::Texture> m;
std::string typingS = "  ";
sf::Font* courier = nullptr;
std::vector<int> stats(3); // 0: load time, 1: check time, 2: memory
std::vector<std::string> sim;
bool trie = false;
int time_counter = -1;
std::chrono::time_point<std::chrono::system_clock> time_start;
Trie::trieNode* curr = nullptr;
Trie* currTrie = nullptr ;
int currInd = -1;

void eventLoop(sf::RenderWindow& w, sf::RenderWindow*& statsW);
void tick(sf::RenderWindow& w);
void render(sf::RenderWindow& w);
void eventLoopStats(sf::RenderWindow*& w);
void tickStats(sf::RenderWindow*& w);
void renderStats(sf::RenderWindow*& w);

void populateTextures() {
    if (!courier->loadFromFile("res/Courier.ttf")) {
        exit(-1);
    }
    sf::Texture menu;
    if (!menu.loadFromFile("res/menu.png")) {
        exit(-1);
    }
    m["menu"] = menu;
    sf::Texture logo;
    if (!logo.loadFromFile("res/logo.png")) {
        exit(-1);
    }
    m["logo"] = logo;
    sf::Texture type;
    if (!type.loadFromFile("res/type.png")) {
        exit(-1);
    }
    m["type"] = type;
    sf::Texture wrong;
    if (!wrong.loadFromFile("res/wrong.png")) {
        exit(-1);
    }
    m["wrong"] = wrong;
    sf::Texture vOver;
    if (!vOver.loadFromFile("res/vOver.png")) {
        exit(-1);
    }
    m["vOver"] = vOver;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(938, 908), "Spell Checker");
    sf::RenderWindow* statsW = nullptr;
    sf::Sprite bgS;
    bg = &bgS;
    sf::Font font;
    courier = &font;
    populateTextures();
    sf::Image icon;
    if (!icon.loadFromFile("res/logo.png")) {
        exit(-1);
    }
    window.setIcon(icon.getSize().x,icon.getSize().y,icon.getPixelsPtr());
    bg->setPosition(0,0);
    bg->setTexture(m["menu"]);
    while (window.isOpen()) {
        eventLoop(window, statsW);
        tick(window);
        render(window);
        eventLoopStats(statsW);
        tickStats(statsW);
        renderStats(statsW);
    }
    m.clear();
    bg = nullptr;
    courier = nullptr;
    if (statsW) {
        statsW->close();
        delete statsW;
    }
    return 0;
}

void eventLoop(sf::RenderWindow& w, sf::RenderWindow*& statsW) {
    sf::Event e;
    while(w.pollEvent(e)) {
        if (e.type == sf::Event::Closed) {
            w.close();
            return;
        }
        if (windowState == 6) {
            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Tilde) {
                    if (statsW) {
                        statsW->close();
                        delete statsW;
                        statsW = nullptr;
                        continue;
                    }
                    statsW = new sf::RenderWindow(sf::VideoMode(400, 500), "Spell Checker Stats");
                    sf::Image icon;
                    if (!icon.loadFromFile("res/logo.png")) {
                        exit(-1);
                    }
                    statsW->setIcon(icon.getSize().x,icon.getSize().y,icon.getPixelsPtr());
                }
            }
            continue;
        }
        if (e.type == sf::Event::TextEntered) {
            if ((windowState+1)/2 == 1 && hide) {
                if ((char) e.text.unicode == '\b' && typingS.length() > 2) {
                    typingS = typingS.substr(0, typingS.length() - 3) + "  ";
                }
                else if (isalpha((char) e.text.unicode)) {
                    typingS = typingS.substr(0, typingS.length() - 2) + (char)tolower(e.text.unicode) + "  ";
                }
            }
        }
        if (e.type == sf::Event::KeyPressed) {
            if ((windowState+1)/2 == 1 && hide) {
                if (e.key.code == sf::Keyboard::Enter && !statsW) windowState += 3;
                else if (e.key.code == sf::Keyboard::Enter) windowState += 5;
                if (windowState == 7) {
                    windowState = 5;
                }
            }
            if (e.key.code == sf::Keyboard::Tilde) {
                if (statsW) {
                    statsW->close();
                    delete statsW;
                    statsW = nullptr;
                    continue;
                }
                statsW = new sf::RenderWindow(sf::VideoMode(400, 500), "Spell Checker Stats");
                sf::Image icon;
                if (!icon.loadFromFile("res/logo.png")) {
                    exit(-1);
                }
                statsW->setIcon(icon.getSize().x,icon.getSize().y,icon.getPixelsPtr());
            }
        }
        if (e.type == sf::Event::MouseButtonReleased) {
            int x = e.mouseButton.x;
            int y = e.mouseButton.y;
            if (e.mouseButton.button == sf::Mouse::Button::Left) {
                if (windowState == 0) {
                    if (y < 610 || y > 758) {
                        continue;
                    }
                    if (x >= 57 && x <= 331) {
                        windowState++;
                        trie = true;
                        bg->setTexture(m["type"]);
                    }
                    else if (x >= 538 && x <= 914) {
                        windowState += 2;
                        trie = false;
                        bg->setTexture(m["type"]);
                    }
                }
                else {
                    if (x <= 175 && y >= 823) {
                        windowState = 0;
                        bg->setTexture(m["menu"]);
                        typingS = "  ";
                        hide = true;
                    }
                    else if (x >= 756 && y >= 823) {
                        if (windowState == 3) {
                            windowState--;
                            windowState -= trie;
                        }
                        hide = true;
                        typingS = "  ";
                    }
                }
            }
            else if (e.mouseButton.button == sf::Mouse::Button::Right) {
            }
        }
    }
}

void tick(sf::RenderWindow& w) {
    if ((windowState+1)/2 == 1) {
        bg->setTexture(m["type"]);
    }
    if (windowState == 4) {
        sim.clear();
        Trie t;
        auto start = std::chrono::system_clock::now();
        t.InsertFromFile("words.txt");
        auto diff = std::chrono::system_clock::now()-start;
        stats[0] = diff.count();
        start = std::chrono::system_clock::now();
        bool check = t.check(typingS.substr(0,typingS.length()-2), sim);
        diff = std::chrono::system_clock::now()-start;
        stats[1] = diff.count();
        stats[2] = t.size*sizeof(Trie::trieNode) + sizeof(Trie);
        hide = false;
        if (check) {
            windowState = 1;
        }
        else {
            windowState = 3;
        }
    }
    if (windowState == 5) {
        sim.clear();
        SpellChecker h;
        auto start = std::chrono::system_clock::now();
        h.loadDictionary("words.txt");
        auto diff = std::chrono::system_clock::now()-start;
        stats[0] = diff.count();
        start = std::chrono::system_clock::now();
        bool check;
        if (!(check = h.checkWord(typingS.substr(0,typingS.length()-2)))) {
            sim = h.getSuggestions(typingS.substr(0,typingS.length()-2));
        }
        diff = std::chrono::system_clock::now()-start;
        stats[1] = diff.count();
        const HashMap<std::string, bool, 100>& m = h.getWordMap();
        // calculations derived from post: https://stackoverflow.com/questions/25375202/how-to-measure-the-memory-usage-of-stdunordered-map
        stats[2] = (m.size() * (sizeof(bool) + sizeof(void*)) + 100 * (sizeof(void*) + sizeof(size_t))) * 1.5;
        hide = false;
        if (check) {
            windowState = 2;
        }
        else {
            windowState = 3;
        }
    }
    if (windowState == 3) {
        bg->setTexture(m["wrong"]);
    }
    if (windowState == 6) {
        hide = true;
        std::chrono::time_point<std::chrono::system_clock> start;
        std::chrono::duration<long long, std::nano> diff;
        if (time_counter == -1) {
            time_start = std::chrono::system_clock::now();
            time_counter = 0;
            stats[1] = 0;
            currTrie = new Trie;
            start = std::chrono::system_clock::now();
            currTrie->InsertFromFile("words.txt");
            diff = std::chrono::system_clock::now() - start;
            stats[0] = diff.count();
            stats[2] = currTrie->size * sizeof(Trie::trieNode) + sizeof(Trie);
        }
        sim.clear();
        time_counter = (std::chrono::system_clock::now()-time_start).count();
        if (time_counter < 1500000000) {
            return;
        }
        start = std::chrono::system_clock::now();
        time_start = std::chrono::system_clock::now();
        currInd++;
        if (currInd == 0) {
            curr = currTrie->children[typingS[0]-97];
        }
        else {
            curr = curr->children[typingS[currInd]-97];
        }
        if (curr == nullptr) {
            windowState = 3;
            hide = false;
            currTrie->doSimilarBottomUp(typingS.substr(0,typingS.length()-2),sim);
        }
        else if (currInd+1 == typingS.length()-2) {
            if (curr->endOfWord) {
                windowState = 1;
            }
            else {
                windowState = 3;
                currTrie->doSimilarBottomUp(typingS.substr(0,typingS.length()-2),sim);
            }
            hide = false;
        }
        diff = std::chrono::system_clock::now() - start;
        stats[1] += diff.count();
        if (!hide) {
            delete currTrie;
            currTrie = nullptr;
            time_counter = -1;
            currInd = -1;
            curr = nullptr;
        }
    }
}

void render(sf::RenderWindow& w) {
    w.clear();
    w.draw(*bg);
    if (windowState == 6) {
        if (hide) {
            sf::RectangleShape hideS;
            hideS.setPosition({160, 600});
            hideS.setSize({640, 160});
            hideS.setFillColor(sf::Color(20, 18, 39));
            w.draw(hideS);
        }
        sf::Text typing;
        typing.setString(typingS.substr(0,typingS.length()-2)+(hide?((time(0)%2) ? "  ":"| "):"  "));
        typing.setFillColor(sf::Color(255,255,255));
        typing.setFont(*courier);
        typing.setStyle(sf::Text::Bold);
        typing.setCharacterSize(80);
        typing.setOrigin(typing.getLocalBounds().width/2, 0);
        typing.setPosition(509, 313);
        w.draw(typing);
    }
    if ((windowState+1)/2 == 1 || windowState == 3) {
        if (hide) {
            sf::RectangleShape hideS;
            hideS.setPosition({160, 600});
            hideS.setSize({640, 160});
            hideS.setFillColor(sf::Color(20, 18, 39));
            w.draw(hideS);
        }
        else if (windowState != 3) {
            sf::RectangleShape hideS;
            hideS.setPosition({105, 465});
            hideS.setSize({729, 79});
            hideS.setFillColor(sf::Color(187, 95, 235));
            w.draw(hideS);
        }

        sf::Text typing;
        typing.setString(typingS.substr(0,typingS.length()-2)+(hide?((time(0)%2) ? "  ":"| "):"  "));
        typing.setFillColor(sf::Color(255,255,255));
        typing.setFont(*courier);
        typing.setStyle(sf::Text::Bold);
        typing.setCharacterSize(80);
        typing.setOrigin(typing.getLocalBounds().width/2, 0);
        typing.setPosition(509, 313);
        w.draw(typing);
        if (windowState == 3) {
            for (int i = 0; i < 3; i++) {
                sf::Text suggested;
                suggested.setString(sim[i]);
                suggested.setFillColor(sf::Color(255, 255, 255));
                suggested.setFont(*courier);
                suggested.setStyle(sf::Text::Bold);
                suggested.setCharacterSize(40);
                suggested.setOrigin(suggested.getGlobalBounds().width / 2, 0);
                suggested.setPosition(w.getSize().x / 2, 700+48*i);
                w.draw(suggested);
            }
        }
    }
    w.display();
}

void eventLoopStats(sf::RenderWindow*& w) {
    if (w == nullptr) {
        return;
    }
    sf::Event e;
    while(w->pollEvent(e)) {
        if (e.type == sf::Event::Closed || (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Tilde)) {
            w->close();
            delete w;
            w = nullptr;
            return;
        }
    }
}

void tickStats(sf::RenderWindow*& w) {
    if (w == nullptr) {
        return;
    }
}

void renderStats(sf::RenderWindow*& w) {
    if (w == nullptr) {
        return;
    }
    w->clear(sf::Color(225,225,255));
    std::string statsTitle[3] = {"Start up time: ","Check/Suggest time: ","Approx. Mem Usage: "};
    for (int i = 0; i < 3; i++) {
        sf::Text statsT;
        statsT.setString(statsTitle[i]+ std::to_string(stats[i]));
        statsT.setFillColor(sf::Color(0,0,0));
        statsT.setFont(*courier);
        statsT.setStyle(sf::Text::Bold);
        statsT.setCharacterSize(20);
        statsT.setOrigin(statsT.getGlobalBounds().width / 2, 0);
        statsT.setPosition(w->getSize().x / 2, 4+28*i);
        w->draw(statsT);
    }
    if (windowState) {
        sf::Text statsT;
        statsT.setString(trie ? "(Trie)" : "(hash)");
        statsT.setFillColor(sf::Color(0, 0, 0));
        statsT.setFont(*courier);
        statsT.setStyle(sf::Text::Bold);
        statsT.setCharacterSize(10);
        statsT.setOrigin(statsT.getGlobalBounds().width / 2, 0);
        statsT.setPosition(w->getSize().x / 2, 88);
        w->draw(statsT);
    }

    sf::RectangleShape bgR({400,400});
    bgR.setPosition(0,100);
    bgR.setFillColor(sf::Color(0,0,0));
    w->draw(bgR);

    bool visual = false;
    std::string noV = "Only Tries have visuals!";
    if (trie) {
        if (windowState != 6) {
            noV = "Enter word to see checker Visual!";
        }
        else {
            visual = true;
        }
    }
    if (windowState == 0) {
        noV = "Check word with Trie for visuals!";
    }
    if (!visual) {
        sf::Text statsT;
        statsT.setString(noV);
        statsT.setFillColor(sf::Color(255,255,255));
        statsT.setFont(*courier);
        statsT.setStyle(sf::Text::Bold);
        statsT.setCharacterSize(15);
        statsT.setOrigin(statsT.getGlobalBounds().width / 2, 0);
        statsT.setPosition(w->getSize().x / 2, 110);
        w->draw(statsT);
    }
    else {
        sf::VertexArray lines(sf::Lines, alphabet_size*2);
        for (int i = 0; i < alphabet_size; i++) {
            lines[i*2].position = {199.5, 155.5};
            lines[i*2+1].position = {199.5, 155.5};
            if (curr && curr->children[i] == nullptr) continue;
            if (!curr && currTrie->children[i] == nullptr) continue;
            lines[i*2].color = lines[i*2+1].color = sf::Color(160,160,160);
            lines[i*2+1].position = {((i-1)%12*32 + (((i-1)/12 - 1) * -32))*1.0f, ((i/13*32) + 304) + 100.0f};
            lines[i*2+1].position += {(i/13)*15.5f,15.5};
            if (i == typingS[currInd+1]-97) {
                lines[i*2].color = lines[i*2+1].color = sf::Color(0,255,0);
            }
            if (i == 0) {
                lines[i*2+1].position = {0, 388};
                lines[i*2+1].position += {15.5, 0};
            }
            if (i == 25) {
                lines[i*2+1].position = {368, 468};
                lines[i*2+1].position += {15.5, 0};
            }
        }
        w->draw(lines);
        sf::Sprite s(m["vOver"]);
        s.setPosition(0,100);
        w->draw(s);
        sf::Text statsT;
        statsT.setString(curr ? std::string()+((char)toupper(curr->letter)) : "ROOT");
        statsT.setFillColor(sf::Color(255,255,255));
        statsT.setFont(*courier);
        statsT.setStyle(sf::Text::Bold);
        statsT.setCharacterSize(40);
        statsT.setOrigin(statsT.getGlobalBounds().width / 2, 0);
        statsT.setPosition({199.5, 132});
        w->draw(statsT);
        sf::Text wordATM;
        wordATM.setString(typingS.substr(0,currInd+1));
        wordATM.setFillColor(sf::Color(255,255,255));
        wordATM.setFont(*courier);
        wordATM.setStyle(sf::Text::Bold);
        wordATM.setCharacterSize(16);
        wordATM.setPosition({0, 100});
        w->draw(wordATM);
    }
    w->display();
}