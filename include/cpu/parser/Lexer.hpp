// Lexer.hpp
//
// A hand-written DFA (deterministic finite automaton) lexer for Cryo.
//
// Each call to Lexer::advance() runs the automaton over the source text and
// returns exactly ONE token. The automaton always starts in State::Start, reads
// one character per step, and picks its next state from (current state,
// character class). It stops when the next character cannot extend the token
// (a "boundary") or when the input ends. The state it stopped in decides the
// token: accepting states produce a token kind, every other state produces
// Token::Kind::Invalid.
//
//   Start --letter-------> Identifier
//   Start --'0'----------> Zero ------b/o/x-----> *Prefix --digit--> *Digits
//   Start --'1'..'9'-----> Decimal --'.'--> DecimalPoint --digit--> Fraction
//   Start --'@'----------> AddressStart --digit--> Zero / Decimal ...
//   Start --anything else> Symbol   (walks symbolAutomaton, longest match wins)
//   any literal state --unexpected character--> Malformed
//
// The lexer only splits text into tokens. It never evaluates expressions and
// never decides whether an identifier is an instruction or a register; that is
// the parser's job.
//
// Extending the lexer:
//   * New operator / punctuation: add one line to detail::symbols. The symbol
//     automaton is rebuilt from that table at compile time.
//   * New literal syntax: add states to Lexer::State, then update
//     nextLiteralState() and kindIfEndedIn(). Compilers warn (-Wswitch) about
//     any switch over State that forgot the new state, so no `default:` is used.
//   * New kind of token starting with a new character: add a branch to the
//     State::Start case in Lexer::advance().

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <string_view>
#include <vector>

#include "cpu/parser/Token.hpp"

namespace cpu::lexer
{

    namespace detail
    {

        // Character classification
        //
        // These predicates are the automaton's input alphabet classes: the
        // transitions below are written in terms of "letter", "binary digit",
        // "number boundary", not in terms of individual characters.

        constexpr bool isWhitespace(char c) noexcept
        {
            return (c == ' ') ||
                   (c == '\t') ||
                   (c == '\n') ||
                   (c == '\r');
        }

        constexpr bool isAlphabetical(char c) noexcept
        {
            return (c >= 'a' && c <= 'z') ||
                   (c >= 'A' && c <= 'Z');
        }

        constexpr bool isAlphanumerical(char c) noexcept
        {
            return (c >= '0' && c <= '9') ||
                   (c >= 'a' && c <= 'z') ||
                   (c >= 'A' && c <= 'Z');
        }

        constexpr bool isDecimal(char c) noexcept
        {
            return c >= '0' && c <= '9';
        }

        constexpr bool isBinary(char c) noexcept
        {
            return c == '0' || c == '1';
        }

        constexpr bool isOctal(char c) noexcept
        {
            return c >= '0' && c <= '7';
        }

        constexpr bool isHexadecimal(char c) noexcept
        {
            return (c >= '0' && c <= '9') ||
                   (c >= 'a' && c <= 'f') ||
                   (c >= 'A' && c <= 'F');
        }

        // Identifier validation

        constexpr bool isIdentifierChar(char c) noexcept
        {
            return isAlphanumerical(c) || c == '_';
        }

        constexpr bool isIdentifierTerminatorChar(char c) noexcept
        {
            return isWhitespace(c) ||
                   c == '\0' ||
                   c == '+' ||
                   c == '-' ||
                   c == '*' ||
                   c == '/' ||
                   c == '%' ||
                   c == '^';
        }

        // Numeric validation
        //
        // A number (or address) token ends at the first character for which this
        // returns true, or at the end of input. Every other character is
        // absorbed into the token, which is what keeps "12abc" or "0xGG" from
        // being split into a valid prefix plus leftovers. Note that '.' and '@'
        // are deliberately NOT boundaries: "1.5.3" and "10@" are malformed.

        constexpr bool isNumberTerminatorChar(char c) noexcept
        {
            return isWhitespace(c) ||
                   c == '\0' ||
                   c == '+' ||
                   c == '-' ||
                   c == '*' ||
                   c == '/' ||
                   c == '%' ||
                   c == '^' ||
                   c == '&' ||
                   c == '|' ||
                   c == '~' ||
                   c == '!' ||
                   c == '=' ||
                   c == '<' ||
                   c == '>' ||
                   c == '(' ||
                   c == ')' ||
                   c == '[' ||
                   c == ']' ||
                   c == '{' ||
                   c == '}' ||
                   c == ',' ||
                   c == ';' ||
                   c == ':';
        }

        struct Symbol
        {
            std::string_view text;
            Token::Kind kind;
        };

        // The single source of truth for operators and punctuation. The symbol
        // automaton below is generated from this table; nothing else lists them.
        inline constexpr Symbol symbols[] =
            {
                // BITWISE
                {"<<", Token::Kind::LeftShift},
                {">>", Token::Kind::RightShift},
                {"&", Token::Kind::BitwiseAnd},
                {"|", Token::Kind::BitwiseOr},
                {"^", Token::Kind::BitwiseXor},
                {"~", Token::Kind::BitwiseNot},

                // COMPARISON
                {"==", Token::Kind::Equal},
                {"!=", Token::Kind::NotEqual},
                {"<", Token::Kind::Less},
                {">", Token::Kind::Greater},
                {"<=", Token::Kind::LessEqual},
                {">=", Token::Kind::GreaterEqual},

                // LOGICAL
                {"&&", Token::Kind::LogicalAnd},
                {"||", Token::Kind::LogicalOr},
                {"!", Token::Kind::LogicalNot},

                // ARITHMETIC
                {"++", Token::Kind::Increment},
                {"--", Token::Kind::Decrement},
                {"+", Token::Kind::Plus},
                {"-", Token::Kind::Minus},
                {"*", Token::Kind::Asterisk},
                {"/", Token::Kind::ForwardSlash},
                {"%", Token::Kind::Percent},

                // GROUPING
                {"(", Token::Kind::LeftParen},
                {")", Token::Kind::RightParen},
                {"[", Token::Kind::LeftBracket},
                {"]", Token::Kind::RightBracket},
                {"{", Token::Kind::LeftBrace},
                {"}", Token::Kind::RightBrace},

                // SEPARATOR
                {",", Token::Kind::Comma},
                {";", Token::Kind::Semicolon},
                {":", Token::Kind::Colon},

                // SPECIAL
                {"->", Token::Kind::ArrowRight},
                {"<-", Token::Kind::ArrowLeft},
                {"<->", Token::Kind::ArrowBoth},
                {"~>", Token::Kind::ArrowTildeRight},
                {"<~", Token::Kind::ArrowTildeLeft},
                {"<~>", Token::Kind::ArrowTildeBoth},
                {"<>", Token::Kind::ArrowAngleBoth},
                {"...", Token::Kind::Ellipsis}

        };

        // Symbol automaton
        //
        // The previous implementation stored the symbols in a trie of heap
        // allocated nodes (unordered_map + unique_ptr) built at program start. A
        // trie already IS a DFA: one node per state, one edge per character. The
        // recognizer therefore keeps the same structure; only its representation
        // changes. The same trie is built at compile time into a flat table with
        // one row per node and one column per possible byte, so a transition is
        // one array load:
        //
        //     nextNode = symbolAutomaton.next[currentNode][byte]
        //
        // Node 0 is the start node. No edge ever leads back to the start node,
        // so the value 0 doubles as "no transition" and no separate dead state
        // is needed. A node is accepting when accepts[node] holds a token kind.

        /// Upper bound on the number of trie nodes: the start node plus at most
        /// one new node per character of every symbol (shared prefixes such as
        /// "<" / "<<" / "<-" / "<->" need fewer, so a few rows stay unused).
        constexpr std::size_t symbolNodeCapacity() noexcept
        {
            std::size_t capacity = 1;

            for (const Symbol &symbol : symbols)
                capacity += symbol.text.size();

            return capacity;
        }

        struct SymbolAutomaton
        {
            static constexpr std::size_t nodeCapacity = symbolNodeCapacity();
            static constexpr std::size_t byteValues =
                std::size_t{std::numeric_limits<unsigned char>::max()} + 1;

            static constexpr std::uint8_t startNode = 0;
            static constexpr std::uint8_t noTransition = 0;

            static_assert(nodeCapacity - 1 <=
                              std::size_t{std::numeric_limits<std::uint8_t>::max()},
                          "Node ids are stored in a std::uint8_t; widen the table type "
                          "if the symbol list grows beyond 256 nodes.");

            std::array<std::array<std::uint8_t, byteValues>, nodeCapacity> next{};
            std::array<std::optional<Token::Kind>, nodeCapacity> accepts{};
        };

        constexpr SymbolAutomaton buildSymbolAutomaton() noexcept
        {
            SymbolAutomaton automaton{};
            std::size_t nodeCount = 1; // node 0 (the start node) already exists

            for (const Symbol &symbol : symbols)
            {
                std::size_t node = SymbolAutomaton::startNode;

                for (const char c : symbol.text)
                {
                    // Index by unsigned value so bytes >= 0x80 (negative when
                    // char is signed) select a valid, always-empty column.
                    const auto column = static_cast<unsigned char>(c);

                    if (automaton.next[node][column] == SymbolAutomaton::noTransition)
                        automaton.next[node][column] = static_cast<std::uint8_t>(nodeCount++);

                    node = automaton.next[node][column];
                }

                automaton.accepts[node] = symbol.kind;
            }

            return automaton;
        }

        // Constant-initialized at compile time: no heap, no hashing, and no
        // dynamic-initialization-order concerns across translation units.
        inline constexpr SymbolAutomaton symbolAutomaton = buildSymbolAutomaton();

    } // namespace detail

    /// Where a token starts in the source. Line and column are 1-based and the
    /// column counts bytes, not Unicode characters.
    struct SourceLocation
    {
        std::size_t offset{};
        std::size_t line{1};
        std::size_t column{1};
    };

    /// Splits Cryo source text into tokens, one per call to advance().
    ///
    /// The lexer does not own the source. Every Token::text() it returns is a
    /// view into the same buffer (nothing is copied or allocated), so the caller
    /// must keep the source alive while the lexer or its tokens are in use.
    ///
    /// Malformed input is reported with Token::Kind::Invalid. The token's text
    /// is the offending characters and locate() gives its position. The lexer
    /// never skips input silently and never reads outside the source.
    class Lexer
    {

    private:
        std::string_view source_{};
        std::size_t position_{};

        /// Every state the automaton can be in while it reads ONE token.
        ///
        /// A state remembers "what the characters consumed so far look like".
        /// Accepting states are the ones where stopping yields a valid token;
        /// kindIfEndedIn() is the single place that lists them. A state that
        /// is not accepting produces Token::Kind::Invalid if the token ends
        /// there (for example "0x", "1." or "@").
        enum class State : std::uint8_t
        {
            /// Initial state, nothing of the token seen yet. Skips
            /// whitespace and chooses the next state from the first
            /// character. Ending here means end of input (EndOfFile).
            Start,

            /// [A-Za-z][A-Za-z0-9_]*   Accepting: Identifier.
            Identifier,

            /// A single leading '0'. Accepting (the literal "0"), and the only
            /// state from which a 0b / 0o / 0x prefix or a "0." fraction can
            /// begin. A further digit ("012") is malformed.
            Zero,

            /// A digit 1-9 followed by decimal digits. Accepting.
            Decimal,

            /// Decimal digits and a '.' ("10.", "0."). NOT accepting: a
            /// fraction needs at least one digit after the point.
            DecimalPoint,

            /// "10.5" - at least one digit after the point. Accepting: Floating.
            Fraction,

            /// "0b" seen, no digit yet. NOT accepting.
            BinaryPrefix,
            /// "0b1" - at least one binary digit. Accepting.
            BinaryDigits,

            /// "0o" seen, no digit yet. NOT accepting.
            OctalPrefix,
            /// "0o7" - at least one octal digit. Accepting.
            OctalDigits,

            /// "0x" seen, no digit yet. NOT accepting.
            HexPrefix,
            /// "0xF" - at least one hexadecimal digit. Accepting.
            HexDigits,

            /// '@' seen, nothing after it yet. NOT accepting. The next
            /// character must start an integer literal; from there the
            /// ordinary numeric states are reused with isAddress == true.
            AddressStart,

            /// Walking symbolAutomaton for an operator or punctuation. One
            /// enum value stands for the whole symbol trie; the position inside
            /// it lives in the symbolNode register of advance(). Also used for
            /// characters that start no token at all (they end up as a
            /// one-character Invalid token, see advance()).
            Symbol,

            /// A literal that has already gone wrong ("0xG", "12abc", "@x").
            /// Never accepting. Absorbs characters up to the next number
            /// boundary so the whole bad literal becomes ONE Invalid token.
            Malformed
        };

        /// @brief Returns the current character, or '\0' at end of input.
        char current() const noexcept
        {
            return position_ < source_.size()
                       ? source_[position_]
                       : '\0';
        }

        /// @brief Returns the current character and moves past it.
        char consume() noexcept
        {
            return position_ < source_.size()
                       ? source_[position_++]
                       : '\0';
        }

        /// The text consumed since `start`. Always valid because the
        /// invariant start <= position_ <= source_.size() holds.
        std::string_view textFrom(std::size_t start) const noexcept
        {
            return source_.substr(start, position_ - start);
        }

        /// The token kind produced when a token ends in `state`: this is the
        /// definition of the accepting states. Every other state returns
        /// Invalid. State::Symbol is not handled here because its result
        /// depends on the symbol trie node (see advance()).
        ///
        /// `isAddress` only changes the answer for the integer states, which
        /// are shared between "0x10" (Number) and "@0x10" (Address).
        static constexpr Token::Kind kindIfEndedIn(State state, bool isAddress) noexcept
        {
            switch (state)
            {
            case State::Identifier:
                return Token::Kind::Identifier;

            case State::Zero:
            case State::Decimal:
            case State::BinaryDigits:
            case State::OctalDigits:
            case State::HexDigits:
                return isAddress ? Token::Kind::Address : Token::Kind::Number;

            case State::Fraction:
                return Token::Kind::Floating;

            case State::Start:
            case State::DecimalPoint:
            case State::BinaryPrefix:
            case State::OctalPrefix:
            case State::HexPrefix:
            case State::AddressStart:
            case State::Symbol:
            case State::Malformed:
                return Token::Kind::Invalid;
            }

            return Token::Kind::Invalid;
        }

        /// The transition function for numbers and addresses: the next state
        /// after reading `c` in `state`. The caller has already checked for a
        /// number boundary, so `c` is known to be part of this token; any
        /// character that does not continue a valid literal sends the
        /// automaton to Malformed.
        ///
        /// Note how each *Prefix state and its *Digits state share the same
        /// outgoing transitions. They differ only in being accepting or not.
        static constexpr State nextLiteralState(State state, char c, bool isAddress) noexcept
        {
            switch (state)
            {
            case State::AddressStart:
                if (c == '0')
                    return State::Zero;
                return detail::isDecimal(c) ? State::Decimal : State::Malformed;

            case State::Zero:
                if (c == 'b')
                    return State::BinaryPrefix;
                if (c == 'o')
                    return State::OctalPrefix;
                if (c == 'x')
                    return State::HexPrefix;
                // Addresses are integers only, so "@0.5" is malformed.
                if (c == '.' && !isAddress)
                    return State::DecimalPoint;
                return State::Malformed;

            case State::Decimal:
                if (detail::isDecimal(c))
                    return State::Decimal;
                if (c == '.' && !isAddress)
                    return State::DecimalPoint;
                return State::Malformed;

            case State::DecimalPoint:
            case State::Fraction:
                return detail::isDecimal(c) ? State::Fraction : State::Malformed;

            case State::BinaryPrefix:
            case State::BinaryDigits:
                return detail::isBinary(c) ? State::BinaryDigits : State::Malformed;

            case State::OctalPrefix:
            case State::OctalDigits:
                return detail::isOctal(c) ? State::OctalDigits : State::Malformed;

            case State::HexPrefix:
            case State::HexDigits:
                return detail::isHexadecimal(c) ? State::HexDigits : State::Malformed;

            // Malformed is absorbing. The remaining states never use this
            // function; they are listed so -Wswitch keeps this exhaustive.
            case State::Malformed:
            case State::Start:
            case State::Identifier:
            case State::Symbol:
                return State::Malformed;
            }

            return State::Malformed;
        }

    public:
        /// @brief Restarts scanning from the beginning of the source.
        void reset() noexcept
        {
            position_ = 0;
        }

        /// @brief The source must outlive the lexer and every token it returns.
        explicit Lexer(std::string_view source) noexcept
            : source_(source)
        {
            // N/A
        }

        /// Scans and returns the next token; EndOfFile once the input is
        /// exhausted (and on every later call).
        ///
        /// One call runs the automaton once, from State::Start. Each loop
        /// iteration reads one character and either consumes it (a transition)
        /// or ends the token without consuming it (a boundary). Characters are
        /// consumed at most once, except in State::Symbol where a failed
        /// longest-match attempt rewinds to the last accepting position.
        Token advance() noexcept
        {
            State state = State::Start;
            std::size_t tokenStart = position_;
            bool isAddress = false;

            // Registers used only in State::Symbol. symbolNode is the current
            // trie node; acceptedEnd / acceptedKind remember the longest
            // accepting prefix seen so far. They start as the fallback "the
            // first character alone is an Invalid token", so a character that
            // starts no symbol is reported instead of skipped.
            std::uint8_t symbolNode = detail::SymbolAutomaton::startNode;
            std::size_t acceptedEnd = position_;
            Token::Kind acceptedKind = Token::Kind::Invalid;

            // Ends the token in the current state. It is called both when a
            // boundary character is reached and when the input runs out, so
            // end of input behaves exactly like one more boundary.
            const auto finishToken = [&]() noexcept -> Token
            {
                if (state == State::Start)
                    return Token{Token::Kind::EndOfFile};

                if (state == State::Symbol)
                {
                    // Give back any characters read past the longest match.
                    position_ = acceptedEnd;
                    return Token{acceptedKind, textFrom(tokenStart)};
                }

                return Token{kindIfEndedIn(state, isAddress), textFrom(tokenStart)};
            };

            while (position_ < source_.size())
            {
                const char c = current();

                switch (state)
                {
                case State::Start:
                {
                    if (detail::isWhitespace(c))
                    {
                        // Whitespace only separates tokens. The token (if any)
                        // starts after it.
                        consume();
                        tokenStart = position_;
                    }
                    else if (detail::isAlphabetical(c))
                    {
                        state = State::Identifier;
                        consume();
                    }
                    else if (c == '0')
                    {
                        state = State::Zero;
                        consume();
                    }
                    else if (detail::isDecimal(c))
                    {
                        state = State::Decimal;
                        consume();
                    }
                    else if (c == '@')
                    {
                        state = State::AddressStart;
                        isAddress = true;
                        consume();
                    }
                    else
                    {
                        // Either the first character of a symbol or a character
                        // that starts no token. It is NOT consumed here: the
                        // Symbol state consumes it if the automaton has an edge
                        // for it. If not, the token ends as the one-character
                        // Invalid token prepared in acceptedEnd / acceptedKind.
                        state = State::Symbol;
                        acceptedEnd = position_ + 1;
                    }
                    break;
                }

                case State::Identifier:
                {
                    // Identifiers have no terminator check: "R1->R2" ends the
                    // identifier at '-' because '-' is not an identifier
                    // character, which is the only rule needed.
                    if (!detail::isIdentifierChar(c))
                        return finishToken();

                    consume();
                    break;
                }

                // Numbers, addresses and malformed literals share ONE boundary
                // rule, so they are handled together. Which of them the token
                // is, is decided by the state, not by this check.
                case State::AddressStart:
                case State::Zero:
                case State::Decimal:
                case State::DecimalPoint:
                case State::Fraction:
                case State::BinaryPrefix:
                case State::BinaryDigits:
                case State::OctalPrefix:
                case State::OctalDigits:
                case State::HexPrefix:
                case State::HexDigits:
                case State::Malformed:
                {
                    if (detail::isNumberTerminatorChar(c))
                        return finishToken();

                    state = nextLiteralState(state, c, isAddress);
                    consume();
                    break;
                }

                case State::Symbol:
                {
                    const std::uint8_t nextNode =
                        detail::symbolAutomaton.next[symbolNode][static_cast<unsigned char>(c)];

                    // The symbol cannot be extended by `c`. Do NOT stop at the
                    // first accepting prefix: "<" may still grow into "<<", "<-"
                    // or "<->". We stop only when the longest match is over.
                    if (nextNode == detail::SymbolAutomaton::noTransition)
                        return finishToken();

                    symbolNode = nextNode;
                    consume();

                    if (const auto kind = detail::symbolAutomaton.accepts[symbolNode])
                    {
                        acceptedEnd = position_;
                        acceptedKind = *kind;
                    }
                    break;
                }
                }
            }

            return finishToken();
        }

        /// Where `token` starts in the source. `token` must have been
        /// returned by this lexer's advance() (EndOfFile maps to the end of
        /// the source). Scans from the start, so call it when reporting an
        /// error rather than for every token.
        [[nodiscard]]
        SourceLocation locate(const Token &token) const noexcept
        {
            std::size_t offset = source_.size();

            if (token.text().data() != nullptr)
            {
                offset = static_cast<std::size_t>(token.text().data() - source_.data());

                if (offset > source_.size())
                    offset = source_.size();
            }

            SourceLocation location{offset, 1, 1};

            for (std::size_t i = 0; i < offset; ++i)
            {
                if (source_[i] == '\n')
                {
                    ++location.line;
                    location.column = 1;
                }
                else
                {
                    ++location.column;
                }
            }

            return location;
        }
    };

    inline std::vector<Token> tokenize(std::string_view source)
    {
        Lexer lexer(source);

        std::vector<Token> tokens;
        tokens.reserve(source.size() / 2);

        while (true)
        {
            Token token = lexer.advance();

            if (token.kind() == Token::Kind::EndOfFile)
                break;

            tokens.push_back(token);
        }

        return tokens;
    }
}
