#include "predictiontrie.h"
#include <string>
#include <queue>
#include <algorithm>

bool comp(MatchedPair& a, MatchedPair& b) {
    if (std::get<1>(a) == std::get<1>(b)) {
        return std::get<0>(a) < std::get<0>(b);
    }
    return std::get<1>(a) > std::get<1>(b);
}

PredictionTrie::PredictionTrie()
{
    _root = new PredictionTrie::PredictionTrieNode;
    _root->count = 0u;
    _root->type = PredictionTrie::PredictionTrieNode::Type::Root;
}

PredictionTrie::~PredictionTrie()
{
    std::queue<PredictionTrieNode*> q;
    q.push(_root);
    while (!q.empty()) {
        auto current = q.front();
        for (auto node: current->children) {
            q.push(node.second);
        }
        delete current;
        q.pop();
    }
}

void PredictionTrie::insert(const std::string& word)
{
    auto* current = _root;
    for (auto letter : word)
    {
        auto foundIt = current->children.find(letter);
        if (foundIt == current->children.end())
        {
            auto [it, _] = current->children.emplace(letter, new PredictionTrieNode);
            foundIt = it;
        }

        current = foundIt->second;
    }

    current->type = PredictionTrie::PredictionTrieNode::Type::Leaf;
    current->count += 1u;
}

void PredictionTrie::remove(const std::string& word) {
    std::string newWord = word;
    const PredictionTrieNode* last = find(word);
    if (last && last->children.empty()) {
        delete last;
        newWord.pop_back();
        last = find(newWord);
        while (last != _root && last->children.size() == 1 && last->type == PredictionTrieNode::Type::Regular) {
            delete last;
            newWord.pop_back();
            last = find(newWord);
        }
        char del = word[newWord.size()];
        auto* x = const_cast<PredictionTrieNode*>(last);
        x->children.erase(del);
    }
    else if (last && !last->children.empty()) {
        auto* q = const_cast<PredictionTrieNode*>(last);
        q->type = PredictionTrieNode::Type::Regular;
        q->count = 0;
    }
}

const PredictionTrie::PredictionTrieNode* PredictionTrie::find(const std::string& word) const
{
    auto* current = _root;
    for (auto letter : word)
    {
        auto foundIt = current->children.find(letter);

        if (foundIt == current->children.end())
        {
            return nullptr;
        }
        current = foundIt->second;
    }
    return current;
}

bool PredictionTrie::isPresented(const std::string& word) const
{
    auto* found = find(word);

    return found && found->type == PredictionTrie::PredictionTrieNode::Type::Leaf;
}

std::vector<MatchedPair> PredictionTrie::allWordsStartedWith(const std::string& wordBegin) const
{
    auto* found = find(wordBegin);
    std::vector<MatchedPair> result;
    if (!found)
    {
        return {};
    }
    else if (found->type == PredictionTrieNode::Type::Leaf) {
        result.emplace_back(wordBegin, found->count);
    }

    collectAllWordsStartedWith(wordBegin, found->children, result);

    return result;
}

void PredictionTrie::collectAllWordsStartedWith(const std::string& wordPart,
        const std::unordered_map<char, PredictionTrieNode*>& letterLayer,
        std::vector<MatchedPair>& result) const {
    for (auto&& [letter, node] : letterLayer) {
        std::string word = wordPart + letter;
        if (node->type == PredictionTrieNode::Type::Leaf) {
            result.emplace_back(word, node->count);
        }
        collectAllWordsStartedWith(word, node->children, result);
    }
}

std::string PredictionTrie::findBestMatch(const std::string& wordBegin) const
{
    return findBestMatches(wordBegin, 1).front();
}

std::vector<std::string> PredictionTrie::findBestMatches(const std::string& wordBegin, unsigned count) const {
    auto allWords = allWordsStartedWith(wordBegin);
    count = allWords.size() < count ? allWords.size() : count;
    std::partial_sort(
            std::begin(allWords),
            allWords.begin() + count,
            std::end(allWords),
            [](const auto& first, const auto& second) {
                return std::get<1>(first) > std::get<1>(second);
            }
    );
    std::vector<std::string> result;
    result.reserve(count);
for (int i = 0; i < count; ++i) {
        result.push_back(std::get<0>(allWords[i]));
    }
    return result;
}

