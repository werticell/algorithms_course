#include <iostream>
#include <vector>
#include <algorithm>
using std::cout;
using std::cin;
using std::vector;

const uint8_t ALPHABET_SIZE = 26;


class Trie {
private:
    struct Vertex {
        Vertex(int32_t parent, char parent_char);
        int32_t edges[ALPHABET_SIZE];
        int32_t go[ALPHABET_SIZE];

        int32_t suffix_link;
        int32_t comp_suffix_link;

        int32_t parent;
        char parent_char;

        bool is_terminal;
        vector<int32_t> pattern_indexes_in_array;
    };
    vector<Vertex> trie_vertexes;
    vector<std::string_view> patterns;

public:
    Trie();

    const Vertex& getVertex(int32_t index) const;
    void add(std::string_view);
    bool contains(std::string_view);

    int32_t getLink(int32_t vertex_number, char symbol);
    int32_t getSuffixLink(int32_t vertex_number);
    int32_t getCompressedSuffixLink(int32_t vertex_number);

    template <typename VisitTrieVertex>
    void checkIfAnySuffixIsTerminal(int32_t vertex_number, int32_t current_pos, VisitTrieVertex visit);
    template <typename VisitTrieVertex>
    void findAllTriePatternsInText(std::string_view str, VisitTrieVertex visit);

};


Trie::Vertex::Vertex(int32_t parent, char parent_char) : parent(parent), parent_char(parent_char),
                                                         is_terminal(false), suffix_link(-1),
                                                         comp_suffix_link(-1){
    std::fill(std::begin(edges), std::end(edges), -1);
    std::fill(std::begin(go), std::end(go), -1);

}


const Trie::Vertex& Trie::getVertex(int32_t index) const {
    return trie_vertexes[index];
}

Trie::Trie() { trie_vertexes.emplace_back(0, -1); }

void Trie::add(std::string_view str) {
    int32_t vertex_number = 0;

    for (auto& symbol : str) {
        int8_t to = symbol - 'a';
        if (trie_vertexes[vertex_number].edges[to] == -1) {
            trie_vertexes.emplace_back(vertex_number, symbol);
            trie_vertexes[vertex_number].edges[to] = trie_vertexes.size() - 1;
        }
        vertex_number = trie_vertexes[vertex_number].edges[to];
    }
    trie_vertexes[vertex_number].is_terminal = true;

    patterns.push_back(str);
    trie_vertexes[vertex_number].pattern_indexes_in_array.push_back(patterns.size() - 1);

}

bool Trie::contains(std::string_view str) {
    int32_t vertex_number = 0;

    for (auto& symbol : str) {
        int8_t to = symbol - 'a';
        if (trie_vertexes[vertex_number].edges[to] == -1)
            return false;
        else
            vertex_number = trie_vertexes[vertex_number].edges[to];
    }
    return trie_vertexes[vertex_number].is_terminal;
}

int32_t Trie::getSuffixLink(int32_t vertex_number) {
    Vertex& current_vertex = trie_vertexes[vertex_number];
    if (current_vertex.suffix_link == -1) {
        if (vertex_number == 0 || current_vertex.parent == 0)
            current_vertex.suffix_link = 0;
        else
            current_vertex.suffix_link = getLink(getSuffixLink(current_vertex.parent),
                                                 static_cast<char>(current_vertex.parent_char - 'a'));
    }
    return current_vertex.suffix_link;
}

int32_t Trie::getLink(int32_t vertex_number, char symbol) {
    Vertex& current_vertex = trie_vertexes[vertex_number];
    if (current_vertex.go[symbol] == -1) {
        if (current_vertex.edges[symbol] != -1)
            current_vertex.go[symbol] = current_vertex.edges[symbol];
        else {
            if (vertex_number == 0)
                current_vertex.go[symbol]  = 0;
            else
                current_vertex.go[symbol] = getLink(getSuffixLink(vertex_number), symbol);
        }
    }
    return current_vertex.go[symbol];

}

int32_t Trie::getCompressedSuffixLink(int32_t vertex_number) {
    Vertex& current_vertex = trie_vertexes[vertex_number];
    if (current_vertex.comp_suffix_link == -1) {
        int32_t some_ancestor = getSuffixLink(vertex_number);
        if (some_ancestor == 0)
            current_vertex.comp_suffix_link = 0;
        else
            current_vertex.comp_suffix_link = (trie_vertexes[some_ancestor].is_terminal) ? some_ancestor :
                                                                                getCompressedSuffixLink(some_ancestor);
    }
    return current_vertex.comp_suffix_link;
}



template <typename VisitTrieVertex>
void Trie::checkIfAnySuffixIsTerminal(int32_t vertex_number, int32_t current_pos_in_text, VisitTrieVertex visit) {
    for (int u = vertex_number; u != 0; u = getCompressedSuffixLink(u)) {
        if (trie_vertexes[u].is_terminal)
            visit(u, current_pos_in_text);
    }
}

template <typename VisitTrieVertex>
void Trie::findAllTriePatternsInText(std::string_view str, VisitTrieVertex visit) {
    int32_t vertex_number = 0;
    for (int32_t current_pos_in_text = 0; current_pos_in_text < str.size(); ++current_pos_in_text) {
        vertex_number = getLink(vertex_number, static_cast<char>(str[current_pos_in_text] - 'a'));
        checkIfAnySuffixIsTerminal(vertex_number, current_pos_in_text, visit);
    }
}

struct SubstringInTemplate {
    SubstringInTemplate(std::string&& substring, int32_t start) : substring(std::forward<std::string&&>(substring)),
                                                                 starting_pos_in_template(start) {}
    std::string substring;
    int32_t starting_pos_in_template;
};

struct VisitTrieVertex {
private:
    const vector<SubstringInTemplate>& substrings_in_template;
    vector<int32_t>& possible_start_of_template;
    const Trie& myTrie;
public:
    VisitTrieVertex(const vector<SubstringInTemplate>& substring_positions,
                    vector<int32_t>& v, const Trie& myTrie) :
            substrings_in_template(substring_positions), possible_start_of_template(v), myTrie(myTrie) {}

    void operator()(int32_t ind, int32_t current_pos) {
        auto& current_vertex= myTrie.getVertex(ind);
        for (int32_t terminal_string_index_in_array : current_vertex.pattern_indexes_in_array) {
            auto& substring_with_pos = substrings_in_template[terminal_string_index_in_array];
            int32_t starting_substring_pos = current_pos - substring_with_pos.substring.size() + 1;

            if (starting_substring_pos - substring_with_pos.starting_pos_in_template >= 0)
                ++possible_start_of_template[starting_substring_pos - substring_with_pos.starting_pos_in_template];
        }
    }
};


vector<SubstringInTemplate> parseTemplate(std::string_view template_string) {
    vector<SubstringInTemplate> result;
    for (int32_t i = 0; i < template_string.size();) {
        std::string substring;
        while (template_string[i] == '?')
                ++i;
        if (i != template_string.size() && template_string[i] != '?') {
            int32_t start = i;
            while (i != template_string.size() && template_string[i] != '?') {
                substring.push_back(template_string[i]);
                ++i;
            }
            result.emplace_back(std::move(substring), start);
        }
    }
    return result;
}



vector<int32_t> findTemplateWithMaskInString(std::string_view template_string, std::string_view text) {
    Trie myTrie;
    vector<SubstringInTemplate> substringPositions = parseTemplate(template_string);
    for (auto& i : substringPositions) // создаем бор из наших подстрок шаблона
        myTrie.add(i.substring);

    vector<int32_t> possible_start_of_template(text.size());
    myTrie.findAllTriePatternsInText(text, VisitTrieVertex(substringPositions, possible_start_of_template, myTrie));

    vector<int32_t> result;
    for (int32_t i = 0; i < text.size(); ++i) {
        if(possible_start_of_template[i] == substringPositions.size() && text.size() - i >= template_string.size())
            result.push_back(i);
    }
    return result;
}



int main() {
    std::string template_string, text;
    cin >> template_string >> text;
    vector<int32_t> answer = findTemplateWithMaskInString(template_string, text);
    for (auto& i : answer)
        cout << i << " ";

    return 0;
}
