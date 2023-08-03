#include "constant.hh"
#include "game_state.hh"
#include "interface.hh"
#include "rules.hh"
#include <iomanip>
#include <ostream>
#include <string_view>
#include <vector>

constexpr auto COMMA = ", ";

/// Decodes a UTF-8 string to a list of 32 bit unicode codepoints. Ignores
/// erroneous characters.
/// Imported from prologin2016
static std::u32string utf8_decode(std::string_view s)
{
    std::u32string ret;
    size_t i = 0;
    size_t size = s.size();

    while (i < size)
    {
        if ((s[i] & 0x80) == 0)
        {
            ret.push_back(s[i++]);
        }
        else if ((s[i] & 0xe0) == 0xc0 && (i + 1) < size &&
                 (s[i + 1] & 0xc0) == 0x80)
        {
            ret.push_back(((s[i] & 0x1f) << 6) | (s[i + 1] & 0x3f));
            i += 2;
        }
        else if ((s[i] & 0xf0) == 0xe0 && (i + 2) < size &&
                 (s[i + 1] & 0xc0) == 0x80 && (s[i + 2] & 0xc0) == 0x80)
        {
            ret.push_back(((s[i] & 0x0f) << 12) | ((s[i + 1] & 0x3f) << 6) |
                          (s[i + 2] & 0x3f));
            i += 3;
        }
        else if ((s[i] & 0xf8) == 0xf0 && (i + 3) < size &&
                 (s[i + 1] & 0xc0) == 0x80 && (s[i + 2] & 0xc0) == 0x80 &&
                 (s[i + 1] & 0xc0) == 0x80)
        {
            ret.push_back(((s[i] & 0x07) << 18) | ((s[i + 1] & 0x3f) << 12) |
                          ((s[i + 2] & 0x3f) << 6) | (s[i + 3] & 0x3f));
            i += 4;
        }
        else
        {
            i++;
        }
    }

    return ret;
}

/// Dump a JSON-escaped UTF-8 string
/// Imported from prologin2016
static void dump_string(std::ostream& ss, std::string_view s)
{
    /*
     * RFC4627, 2.5:
     * All Unicode characters may be placed within the quotation marks except
     * for the characters that must be escaped: quotation mark, reverse solidus,
     * and the control characters (U+0000 through U+001F).
     */
    std::ios state(nullptr);
    state.copyfmt(ss);
    std::u32string utf32 = utf8_decode(s);
    ss << "\"";
    for (char32_t& c : utf32)
    {
        if (c == u'"')
        {
            ss << "\\\"";
        }
        else if (c == u'\\')
        {
            ss << "\\\\";
        }
        else if (u'\u0020' <= c && c <= u'\u007E')
        {
            // printable ASCII
            ss << static_cast<char>(c);
        }
        else if (c > u'\uFFFF')
        {
            // surrogate pair
            // http://unicode.org/faq/utf_bom.html#utf16-2
            const unsigned s = c - 0x010000;
            const unsigned lead = (s >> 10) + 0xD800;
            const unsigned trail = (s & 0x3FF) + 0xDC00;
            ss << "\\u" << std::hex << std::setfill('0') << std::setw(4)
               << lead;
            ss.copyfmt(state);
            ss << "\\u" << std::hex << std::setfill('0') << std::setw(4)
               << trail;
            ss.copyfmt(state);
        }
        else
        {
            ss << "\\u" << std::hex << std::setfill('0') << std::setw(4) << c;
            ss.copyfmt(state);
        }
    }
    ss << '"';
}

struct Str
{
    Str(std::string_view str)
        : str(str)
    {
    }

    std::string_view str;
};

static std::ostream& operator<<(std::ostream& ss, Str str)
{
    dump_string(ss, str.str);

    return ss;
}

// Enums.
// ===========================================================================

// Basic structs.
// ===========================================================================

template <typename V>
struct KV
{
    KV(std::string_view key, const V& value)
        : key(key)
        , value(value)
    {
    }

    std::string_view key;
    const V& value;
};

template <typename V>
struct PAR
{
    PAR(const V& value)
        : value(value)
    {
    }

    const V& value;
};

template <typename V>
static std::ostream& operator<<(std::ostream& ss, KV<V> kv)
{
    return ss << Str(kv.key) << ": " << kv.value;
}
template <typename V>
static std::ostream& operator<<(std::ostream& ss, PAR<V> v)
{
    return ss << "\"" << v.value << "\"";
}

template <>
std::ostream& operator<<(std::ostream& ss, KV<bool> kv)
{
    return ss << Str(kv.key) << ": " << (kv.value ? "true" : "false");
}

template <typename T>
struct Vec
{
    Vec(const std::vector<T>& vec)
        : vec(vec)
    {
    }

    const std::vector<T>& vec;
};

// Large classes.
// ===========================================================================

template <typename T>
static std::ostream& operator<<(std::ostream& ss, Vec<T> vec)
{
    ss << '[';

    const auto& values = vec.vec;

    for (size_t i = 0; i < values.size(); i++)
    {
        ss << values[i];

        if (i < values.size() - 1)
            ss << ", ";
    }

    return ss << ']';
}

void Rules::dump_state(std::ostream& ss)
{
    ss << api_->game_state();
}

std::ostream& operator<<(std::ostream& ss, const action_jouee& aj)
{
    ss << "{" << KV{"action", PAR{aj.act}};
    switch (aj.act)
    {
    case VALIDER:
        ss << ", " << KV{"cartes", Vec(std::vector({aj.c1}))};
        break;
    case DEFAUSSER:
        ss << ", " << KV{"cartes", Vec(std::vector({aj.c1, aj.c2}))};
        break;
    case CHOIX_TROIS:
        ss << ", " << KV{"cartes", Vec(std::vector({aj.c1, aj.c2, aj.c3}))};
        break;
    case CHOIX_PAQUETS:
        ss << ", "
           << KV{"cartes", Vec(std::vector({aj.c1, aj.c2, aj.c3, aj.c4}))};
        break;
    default:
        break;
    }

    ss << "}";
    return ss;
}

std::ostream& operator<<(std::ostream& ss, cardset set)
{
    std::vector<int> cartes = cardset_to_vector(set);
    return ss << Vec(cartes);
}

bool hasStarted = false;
std::ostream& operator<<(std::ostream& ss, const GameState& gs)
{
    if (hasStarted)
        ss << ",\n";
    else
        ss << "[\n";

    hasStarted = true;
    ss << "{";

    ss << KV{"manche", gs.m_manche} << ", " << KV{"tour", gs.m_tour} << ", "
       << KV{"attente_reponse", gs.m_attente_reponse} << ", "
       << KV{"dernier_choix", gs.m_dernier_choix} << ", "
       << KV{"derniere_action", gs.m_derniere_action};

    if (!gs.fini())
    {
        ss << ", "
           << KV{"carte_ecartee",
                 gs.m_pioches[(gs.m_manche + 1) * NB_CARTES_TOTAL - 1]}
           << ", "
           << KV{"cartes_pioche",
                 Vec(std::vector(
                     gs.m_pioches + gs.m_manche * NB_CARTES_TOTAL +
                         NB_JOUEURS * NB_CARTES_DEBUT + gs.tour() + 1,
                     gs.m_pioches + (gs.m_manche + 1) * NB_CARTES_TOTAL))};
    }

    for (int i = 0; i < NB_JOUEURS; i++)
    {
        ss << ", \"joueur_" << i << "\": {";

        ss << KV{"id", gs.players_[i]->id} << ", ";
        ss << KV{"nom", PAR(gs.players_[i]->name)} << ", ";
        ss << KV{"score", gs.players_[i]->score} << ", ";
        ss << KV{"main", gs.m_joueurs_main[i]} << ", ";
        ss << KV{"validees", gs.m_joueurs_validee[i]} << ", ";
        ss << KV{"validees_secretement", gs.m_joueurs_validee_secretement[i]};

        ss << "}";
    }

    ss << "}";
    return ss;
}