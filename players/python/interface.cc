// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2020 Association Prologin <association@prologin.org>

// This file contains the code to call the API functions from the Python
// language.
// This file was generated by stechec2-generator. DO NOT EDIT.

#include <Python.h>
#include <vector>
#include <string>

static PyObject* py_module;
static PyObject* champ_module;


/// Les actions de jeu
typedef enum action
{
    VALIDER, ///< Valide une unique carte
    DEFAUSSER, ///< Defausse deux cartes
    CHOIX_TROIS, ///< Donne le choix entre trois cartes
    CHOIX_PAQUETS, ///< Donne le choix entre deux paquets de deux cartes
    PREMIER_JOUEUR, ///< Aucune action n'a été jouée (utilisé dans tour_precedant)
} action;

/// Enumeration contentant toutes les erreurs possibles
typedef enum error
{
    OK, ///< pas d'erreur
    ACTION_DEJA_JOUEE, ///< l'action a déjà été jouée
    CARTES_INVALIDES, ///< vous ne pouvez pas jouer ces cartes
    PAQUET_INVALIDE, ///< ce paquet n'existe pas
    GEISHA_INVALIDES, ///< cette geisha n'existe pas (doit être un entier entre 0 et NB_GEISHA)
    JOUEUR_INVALIDE, ///< ce joueur n'existe pas
    CHOIX_INVALIDE, ///< vous ne pouvez pas repondre à ce choix
    ACTION_INVALIDE, ///< vous ne pouvez pas jouer cette action maintenant
} error;

/// Enumeration représentant les différents joueurs
typedef enum joueur
{
    JOUEUR_1, ///< Le joueur 1
    JOUEUR_2, ///< Le joueur 2
    EGALITE, ///< Égalité, utilisé uniquement dans possession_geisha
} joueur;

/// La description d'une action jouée
typedef struct action_jouee
{
    action act; ///< L'action jouée
    int c1; ///< Si act==VALIDER ou act==DEFAUSSER, -1 sinon la première carte (du premier paquet)
    int c2; ///< Si act==V|D: -1 sinon la deuxième carte (du premier paquet)
    int c3; ///< Si act==V|D: -1 sinon la troisième carte (ou la première carte du second paquet si act==choix paquet)
    int c4; ///< Si act!=choix paquet: -1 sinon la deuxième carte du second paquet
} action_jouee;

extern "C" {

/// Renvoie l'identifiant du joueur
joueur api_id_joueur();

/// Renvoie l'identifiant de l'adversaire
joueur api_id_adversaire();

/// Renvoie le numéro de la manche
int api_manche();

/// Renvoie le numéro de la manche
int api_tour();

/// Renvoie l'action jouée par l'adversaire
action_jouee api_tour_precedant();

/// Renvoie le nombre de carte validée par le joueur pour la geisha
int api_nb_carte_validee(joueur j, int g);

/// Renvoie qui possède la geisha
joueur api_possession_geisha(int g);

/// Renvoie si l'action a déjà été jouée par le joueur
bool api_est_jouee_action(joueur j, action a);

/// Renvoie le nombre de carte que chaque joueur a
int api_nb_cartes(joueur j);

/// Renvoie les cartes que vous avez
std::vector<int> api_cartes_en_main();

/// Jouer l'action valider une carte
error api_action_valider(int c);

/// Jouer l'action défausser deux cartes
error api_action_defausser(int c1, int c2);

/// Jouer l'action choisir entre trois cartes
error api_action_choix_trois(int c1, int c2, int c3);

/// Jouer l'action choisir entre deux paquets de deux cartes
error api_action_choix_paquets(int p1c1, int p1c2, int p2c1, int p2c2);

/// Choisir une des trois cartes proposées.
error api_repondre_choix_trois(int c);

/// Choisir un des deux paquets proposés.
error api_repondre_choix_paquets(int p);

/// Affiche le contenu d'une valeur de type action
void api_afficher_action(action v);

/// Affiche le contenu d'une valeur de type error
void api_afficher_error(error v);

/// Affiche le contenu d'une valeur de type joueur
void api_afficher_joueur(joueur v);

/// Affiche le contenu d'une valeur de type action_jouee
void api_afficher_action_jouee(action_jouee v);
}

template <typename PythonType, typename CxxType>
PythonType cxx_to_python(CxxType in)
{
    return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
PyObject* cxx_to_python<PyObject*, int>(int in)
{
    return PyLong_FromLong(in);
}


template <>
PyObject* cxx_to_python<PyObject*, double>(double in)
{
    return PyFloat_FromDouble(in);
}


template <>
PyObject* cxx_to_python<PyObject*, std::string>(std::string in)
{
    return PyUnicode_FromString(in.c_str());
}


template <>
PyObject* cxx_to_python<PyObject*, bool>(bool in)
{
    return PyBool_FromLong(in);
}

template <typename CxxType>
PyObject* cxx_to_python_array(const std::vector<CxxType>& in)
{
    size_t size = in.size();
    PyObject* out = PyList_New(size);

    for (unsigned int i = 0; i < size; ++i)
        PyList_SET_ITEM(out, i, (cxx_to_python<PyObject*, CxxType>(in[i])));

    return out;
}

template <typename PythonType, typename CxxType>
CxxType python_to_cxx(PythonType in)
{
    return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
int python_to_cxx<PyObject*, int>(PyObject* in)
{
    long out = PyLong_AsLong(in);
    if (PyErr_Occurred())
        throw 42;
    return out;
}

template <>
double python_to_cxx<PyObject*, double>(PyObject* in)
{
    double out = PyFloat_AsDouble(in);
    if (PyErr_Occurred())
        throw 42;
    return out;
}

template <>
bool python_to_cxx<PyObject*, bool>(PyObject* in)
{
    return static_cast<bool>(python_to_cxx<PyObject*, int>(in));
}

template <>
std::string python_to_cxx<PyObject*, std::string>(PyObject* in)
{
    const char* out = PyUnicode_AsUTF8(in);
    if (PyErr_Occurred())
        throw 42;
    return out;
}

template <typename CxxType>
std::vector<CxxType> python_to_cxx_array(PyObject* in)
{
    if (!PyList_Check(in))
    {
        PyErr_SetString(PyExc_TypeError, "a list is required");
        throw 42;
    }

    std::vector<CxxType> out;
    unsigned int size = PyList_Size(in);
    out.reserve(size);

    for (unsigned int i = 0; i < size; ++i)
        out.push_back(python_to_cxx<PyObject*, CxxType>(PyList_GET_ITEM(in, i)));

    return out;
}

// Les actions de jeu

template<>
PyObject* cxx_to_python<PyObject*, action>(action in)
{
    PyObject* name = PyUnicode_FromString("action");
    PyObject* enm = PyObject_GetAttr(py_module, name);
    Py_DECREF(name);
    if (enm == nullptr)
        throw 42;
    PyObject* arglist = Py_BuildValue("(i)", static_cast<int>(in));
    PyObject* ret = PyObject_CallObject(enm, arglist);
    Py_DECREF(enm);
    Py_DECREF(arglist);
    return ret;
}

template <>
action python_to_cxx<PyObject*, action>(PyObject* in)
{
    return static_cast<action>(python_to_cxx<PyObject*, int>(in));
}

// Enumeration contentant toutes les erreurs possibles

template<>
PyObject* cxx_to_python<PyObject*, error>(error in)
{
    PyObject* name = PyUnicode_FromString("error");
    PyObject* enm = PyObject_GetAttr(py_module, name);
    Py_DECREF(name);
    if (enm == nullptr)
        throw 42;
    PyObject* arglist = Py_BuildValue("(i)", static_cast<int>(in));
    PyObject* ret = PyObject_CallObject(enm, arglist);
    Py_DECREF(enm);
    Py_DECREF(arglist);
    return ret;
}

template <>
error python_to_cxx<PyObject*, error>(PyObject* in)
{
    return static_cast<error>(python_to_cxx<PyObject*, int>(in));
}

// Enumeration représentant les différents joueurs

template<>
PyObject* cxx_to_python<PyObject*, joueur>(joueur in)
{
    PyObject* name = PyUnicode_FromString("joueur");
    PyObject* enm = PyObject_GetAttr(py_module, name);
    Py_DECREF(name);
    if (enm == nullptr)
        throw 42;
    PyObject* arglist = Py_BuildValue("(i)", static_cast<int>(in));
    PyObject* ret = PyObject_CallObject(enm, arglist);
    Py_DECREF(enm);
    Py_DECREF(arglist);
    return ret;
}

template <>
joueur python_to_cxx<PyObject*, joueur>(PyObject* in)
{
    return static_cast<joueur>(python_to_cxx<PyObject*, int>(in));
}

// La description d'une action jouée

template <>
PyObject* cxx_to_python<PyObject*, action_jouee>(action_jouee in)
{
    PyObject* tuple = PyTuple_New(5);
    PyTuple_SET_ITEM(tuple, 0, (cxx_to_python<PyObject*, action>(in.act)));
    PyTuple_SET_ITEM(tuple, 1, (cxx_to_python<PyObject*, int>(in.c1)));
    PyTuple_SET_ITEM(tuple, 2, (cxx_to_python<PyObject*, int>(in.c2)));
    PyTuple_SET_ITEM(tuple, 3, (cxx_to_python<PyObject*, int>(in.c3)));
    PyTuple_SET_ITEM(tuple, 4, (cxx_to_python<PyObject*, int>(in.c4)));
    PyObject* name = PyUnicode_FromString("action_jouee");
    PyObject* cstr = PyObject_GetAttr(py_module, name);
    Py_DECREF(name);
    if (cstr == nullptr)
        throw 42;
    PyObject* ret = PyObject_CallObject(cstr, tuple);
    Py_DECREF(cstr);
    Py_DECREF(tuple);
    if (ret == nullptr)
        throw 42;
    return ret;
}

template <>
action_jouee python_to_cxx<PyObject*, action_jouee>(PyObject* in)
{
    action_jouee out;
    PyObject* i;

    // L'action jouée
    i = cxx_to_python<PyObject*, int>(0);
    i = PyObject_GetItem(in, i);
    if (i == nullptr)
        throw 42;
    out.act = python_to_cxx<PyObject*, action>(i);
    Py_DECREF(i);

    // Si act==VALIDER ou act==DEFAUSSER, -1 sinon la première carte (du premier
// paquet)
    i = cxx_to_python<PyObject*, int>(1);
    i = PyObject_GetItem(in, i);
    if (i == nullptr)
        throw 42;
    out.c1 = python_to_cxx<PyObject*, int>(i);
    Py_DECREF(i);

    // Si act==V|D: -1 sinon la deuxième carte (du premier paquet)
    i = cxx_to_python<PyObject*, int>(2);
    i = PyObject_GetItem(in, i);
    if (i == nullptr)
        throw 42;
    out.c2 = python_to_cxx<PyObject*, int>(i);
    Py_DECREF(i);

    // Si act==V|D: -1 sinon la troisième carte (ou la première carte du second
// paquet si act==choix paquet)
    i = cxx_to_python<PyObject*, int>(3);
    i = PyObject_GetItem(in, i);
    if (i == nullptr)
        throw 42;
    out.c3 = python_to_cxx<PyObject*, int>(i);
    Py_DECREF(i);

    // Si act!=choix paquet: -1 sinon la deuxième carte du second paquet
    i = cxx_to_python<PyObject*, int>(4);
    i = PyObject_GetItem(in, i);
    if (i == nullptr)
        throw 42;
    out.c4 = python_to_cxx<PyObject*, int>(i);
    Py_DECREF(i);

    return out;
}


// Python native wrapper for function id_joueur.
// Renvoie l'identifiant du joueur
static PyObject* p_id_joueur(PyObject* /* self */, PyObject* args)
{
    if (!PyArg_ParseTuple(args, ""))
    {
        return nullptr;
    }

    try {
        return cxx_to_python<PyObject*, joueur>(api_id_joueur());
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function id_adversaire.
// Renvoie l'identifiant de l'adversaire
static PyObject* p_id_adversaire(PyObject* /* self */, PyObject* args)
{
    if (!PyArg_ParseTuple(args, ""))
    {
        return nullptr;
    }

    try {
        return cxx_to_python<PyObject*, joueur>(api_id_adversaire());
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function manche.
// Renvoie le numéro de la manche
static PyObject* p_manche(PyObject* /* self */, PyObject* args)
{
    if (!PyArg_ParseTuple(args, ""))
    {
        return nullptr;
    }

    try {
        return cxx_to_python<PyObject*, int>(api_manche());
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function tour.
// Renvoie le numéro de la manche
static PyObject* p_tour(PyObject* /* self */, PyObject* args)
{
    if (!PyArg_ParseTuple(args, ""))
    {
        return nullptr;
    }

    try {
        return cxx_to_python<PyObject*, int>(api_tour());
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function tour_precedant.
// Renvoie l'action jouée par l'adversaire
static PyObject* p_tour_precedant(PyObject* /* self */, PyObject* args)
{
    if (!PyArg_ParseTuple(args, ""))
    {
        return nullptr;
    }

    try {
        return cxx_to_python<PyObject*, action_jouee>(api_tour_precedant());
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function nb_carte_validee.
// Renvoie le nombre de carte validée par le joueur pour la geisha
static PyObject* p_nb_carte_validee(PyObject* /* self */, PyObject* args)
{
    PyObject* arg_j;
    PyObject* arg_g;
    if (!PyArg_ParseTuple(args, "OO", &arg_j, &arg_g))
    {
        return nullptr;
    }

    try {
        return cxx_to_python<PyObject*, int>(api_nb_carte_validee(python_to_cxx<PyObject*, joueur>(arg_j), python_to_cxx<PyObject*, int>(arg_g)));
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function possession_geisha.
// Renvoie qui possède la geisha
static PyObject* p_possession_geisha(PyObject* /* self */, PyObject* args)
{
    PyObject* arg_g;
    if (!PyArg_ParseTuple(args, "O", &arg_g))
    {
        return nullptr;
    }

    try {
        return cxx_to_python<PyObject*, joueur>(api_possession_geisha(python_to_cxx<PyObject*, int>(arg_g)));
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function est_jouee_action.
// Renvoie si l'action a déjà été jouée par le joueur
static PyObject* p_est_jouee_action(PyObject* /* self */, PyObject* args)
{
    PyObject* arg_j;
    PyObject* arg_a;
    if (!PyArg_ParseTuple(args, "OO", &arg_j, &arg_a))
    {
        return nullptr;
    }

    try {
        return cxx_to_python<PyObject*, bool>(api_est_jouee_action(python_to_cxx<PyObject*, joueur>(arg_j), python_to_cxx<PyObject*, action>(arg_a)));
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function nb_cartes.
// Renvoie le nombre de carte que chaque joueur a
static PyObject* p_nb_cartes(PyObject* /* self */, PyObject* args)
{
    PyObject* arg_j;
    if (!PyArg_ParseTuple(args, "O", &arg_j))
    {
        return nullptr;
    }

    try {
        return cxx_to_python<PyObject*, int>(api_nb_cartes(python_to_cxx<PyObject*, joueur>(arg_j)));
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function cartes_en_main.
// Renvoie les cartes que vous avez
static PyObject* p_cartes_en_main(PyObject* /* self */, PyObject* args)
{
    if (!PyArg_ParseTuple(args, ""))
    {
        return nullptr;
    }

    try {
        return cxx_to_python_array(api_cartes_en_main());
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function action_valider.
// Jouer l'action valider une carte
static PyObject* p_action_valider(PyObject* /* self */, PyObject* args)
{
    PyObject* arg_c;
    if (!PyArg_ParseTuple(args, "O", &arg_c))
    {
        return nullptr;
    }

    try {
        return cxx_to_python<PyObject*, error>(api_action_valider(python_to_cxx<PyObject*, int>(arg_c)));
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function action_defausser.
// Jouer l'action défausser deux cartes
static PyObject* p_action_defausser(PyObject* /* self */, PyObject* args)
{
    PyObject* arg_c1;
    PyObject* arg_c2;
    if (!PyArg_ParseTuple(args, "OO", &arg_c1, &arg_c2))
    {
        return nullptr;
    }

    try {
        return cxx_to_python<PyObject*, error>(api_action_defausser(python_to_cxx<PyObject*, int>(arg_c1), python_to_cxx<PyObject*, int>(arg_c2)));
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function action_choix_trois.
// Jouer l'action choisir entre trois cartes
static PyObject* p_action_choix_trois(PyObject* /* self */, PyObject* args)
{
    PyObject* arg_c1;
    PyObject* arg_c2;
    PyObject* arg_c3;
    if (!PyArg_ParseTuple(args, "OOO", &arg_c1, &arg_c2, &arg_c3))
    {
        return nullptr;
    }

    try {
        return cxx_to_python<PyObject*, error>(api_action_choix_trois(python_to_cxx<PyObject*, int>(arg_c1), python_to_cxx<PyObject*, int>(arg_c2), python_to_cxx<PyObject*, int>(arg_c3)));
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function action_choix_paquets.
// Jouer l'action choisir entre deux paquets de deux cartes
static PyObject* p_action_choix_paquets(PyObject* /* self */, PyObject* args)
{
    PyObject* arg_p1c1;
    PyObject* arg_p1c2;
    PyObject* arg_p2c1;
    PyObject* arg_p2c2;
    if (!PyArg_ParseTuple(args, "OOOO", &arg_p1c1, &arg_p1c2, &arg_p2c1, &arg_p2c2))
    {
        return nullptr;
    }

    try {
        return cxx_to_python<PyObject*, error>(api_action_choix_paquets(python_to_cxx<PyObject*, int>(arg_p1c1), python_to_cxx<PyObject*, int>(arg_p1c2), python_to_cxx<PyObject*, int>(arg_p2c1), python_to_cxx<PyObject*, int>(arg_p2c2)));
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function repondre_choix_trois.
// Choisir une des trois cartes proposées.
static PyObject* p_repondre_choix_trois(PyObject* /* self */, PyObject* args)
{
    PyObject* arg_c;
    if (!PyArg_ParseTuple(args, "O", &arg_c))
    {
        return nullptr;
    }

    try {
        return cxx_to_python<PyObject*, error>(api_repondre_choix_trois(python_to_cxx<PyObject*, int>(arg_c)));
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function repondre_choix_paquets.
// Choisir un des deux paquets proposés.
static PyObject* p_repondre_choix_paquets(PyObject* /* self */, PyObject* args)
{
    PyObject* arg_p;
    if (!PyArg_ParseTuple(args, "O", &arg_p))
    {
        return nullptr;
    }

    try {
        return cxx_to_python<PyObject*, error>(api_repondre_choix_paquets(python_to_cxx<PyObject*, int>(arg_p)));
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function afficher_action.
// Affiche le contenu d'une valeur de type action
static PyObject* p_afficher_action(PyObject* /* self */, PyObject* args)
{
    PyObject* arg_v;
    if (!PyArg_ParseTuple(args, "O", &arg_v))
    {
        return nullptr;
    }

    try {
        (api_afficher_action(python_to_cxx<PyObject*, action>(arg_v)));
        Py_RETURN_NONE;
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function afficher_error.
// Affiche le contenu d'une valeur de type error
static PyObject* p_afficher_error(PyObject* /* self */, PyObject* args)
{
    PyObject* arg_v;
    if (!PyArg_ParseTuple(args, "O", &arg_v))
    {
        return nullptr;
    }

    try {
        (api_afficher_error(python_to_cxx<PyObject*, error>(arg_v)));
        Py_RETURN_NONE;
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function afficher_joueur.
// Affiche le contenu d'une valeur de type joueur
static PyObject* p_afficher_joueur(PyObject* /* self */, PyObject* args)
{
    PyObject* arg_v;
    if (!PyArg_ParseTuple(args, "O", &arg_v))
    {
        return nullptr;
    }

    try {
        (api_afficher_joueur(python_to_cxx<PyObject*, joueur>(arg_v)));
        Py_RETURN_NONE;
    } catch (...) {
        return nullptr;
    }
}

// Python native wrapper for function afficher_action_jouee.
// Affiche le contenu d'une valeur de type action_jouee
static PyObject* p_afficher_action_jouee(PyObject* /* self */, PyObject* args)
{
    PyObject* arg_v;
    if (!PyArg_ParseTuple(args, "O", &arg_v))
    {
        return nullptr;
    }

    try {
        (api_afficher_action_jouee(python_to_cxx<PyObject*, action_jouee>(arg_v)));
        Py_RETURN_NONE;
    } catch (...) {
        return nullptr;
    }
}

// API function to register.
static PyMethodDef api_callback[] = {
    {"id_joueur", p_id_joueur, METH_VARARGS, "id_joueur"},
    {"id_adversaire", p_id_adversaire, METH_VARARGS, "id_adversaire"},
    {"manche", p_manche, METH_VARARGS, "manche"},
    {"tour", p_tour, METH_VARARGS, "tour"},
    {"tour_precedant", p_tour_precedant, METH_VARARGS, "tour_precedant"},
    {"nb_carte_validee", p_nb_carte_validee, METH_VARARGS, "nb_carte_validee"},
    {"possession_geisha", p_possession_geisha, METH_VARARGS, "possession_geisha"},
    {"est_jouee_action", p_est_jouee_action, METH_VARARGS, "est_jouee_action"},
    {"nb_cartes", p_nb_cartes, METH_VARARGS, "nb_cartes"},
    {"cartes_en_main", p_cartes_en_main, METH_VARARGS, "cartes_en_main"},
    {"action_valider", p_action_valider, METH_VARARGS, "action_valider"},
    {"action_defausser", p_action_defausser, METH_VARARGS, "action_defausser"},
    {"action_choix_trois", p_action_choix_trois, METH_VARARGS, "action_choix_trois"},
    {"action_choix_paquets", p_action_choix_paquets, METH_VARARGS, "action_choix_paquets"},
    {"repondre_choix_trois", p_repondre_choix_trois, METH_VARARGS, "repondre_choix_trois"},
    {"repondre_choix_paquets", p_repondre_choix_paquets, METH_VARARGS, "repondre_choix_paquets"},
    {"afficher_action", p_afficher_action, METH_VARARGS, "afficher_action"},
    {"afficher_error", p_afficher_error, METH_VARARGS, "afficher_error"},
    {"afficher_joueur", p_afficher_joueur, METH_VARARGS, "afficher_joueur"},
    {"afficher_action_jouee", p_afficher_action_jouee, METH_VARARGS, "afficher_action_jouee"},
    {nullptr, nullptr, 0, nullptr}
};

// Initialize C module.
PyMODINIT_FUNC PyInit__api()
{
    static struct PyModuleDef apimoduledef = {
        PyModuleDef_HEAD_INIT,
        "_api",
        "API module",
        -1,
        api_callback,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
    };
    return PyModule_Create(&apimoduledef);
}

// Load a Python module.
static PyObject* _import_module(const char* m)
{
    PyObject* name = PyUnicode_FromString(m);
    PyObject* mod = PyImport_Import(name);
    Py_DECREF(name);
    if (mod == nullptr)
        if (PyErr_Occurred())
        {
            PyErr_Print();
            abort();
        }
    return mod;
}

// Initialize Python VM, register API functions, and load .py file.
static void _init_python()
{
    static wchar_t empty_string[] = L"";
    static wchar_t *argv[] = { (wchar_t *) &empty_string, nullptr };

    const char* champion_path;

    champion_path = getenv("CHAMPION_PATH");
    if (champion_path == nullptr)
        champion_path = ".";

    setenv("PYTHONPATH", champion_path, 1);

    static wchar_t program_name[] = L"stechec";
    Py_SetProgramName(program_name);

    PyImport_AppendInittab("_api", PyInit__api);
    Py_Initialize();
    PySys_SetArgvEx(1, argv, 0);

    // Import a Champion module with an uppercase letter to avoid conflict with
    // champion.so
    champ_module = _import_module("Champion");
    py_module = _import_module("api");
}

// Flush stdout and stderr
static void flush_std_files()
{
    PyObject *fout = PySys_GetObject("stdout");
    PyObject *ferr = PySys_GetObject("stderr");
    PyObject *tmp;

    if (fout != nullptr && fout != Py_None) {
        tmp = PyObject_CallMethod(fout, "flush", "");
        if (tmp == nullptr)
            PyErr_WriteUnraisable(fout);
        else
            Py_DECREF(tmp);
    }

    if (ferr != nullptr && ferr != Py_None) {
        tmp = PyObject_CallMethod(ferr, "flush", "");
        if (tmp == nullptr)
            PyErr_Clear();
        else
            Py_DECREF(tmp);
    }
}

// Run a Python function.
static PyObject* _call_python_function(const char* name)
{
    static bool initialized = false;

    if (!initialized)
    {
        initialized = true;
        _init_python();
    }

    PyObject *arglist, *func;
    PyObject *result = nullptr;

    func = PyObject_GetAttrString(champ_module, name);
    if (func && PyCallable_Check(func))
    {
        arglist = Py_BuildValue("()");
        result = PyEval_CallObject(func, arglist);
        Py_XDECREF(arglist);
        Py_DECREF(func);
    }
    if (result == nullptr && PyErr_Occurred())
    {
        PyErr_Print();
        abort();
    }

    flush_std_files();
    return result;
}


// Functions called from Stechec to Python.

extern "C" void init_jeu()
{
    PyObject* _retval = _call_python_function("init_jeu");
    try {
        Py_XDECREF(_retval);
    } catch (...) {
        PyErr_Print();
        abort();
    }
}

extern "C" void jouer_tour()
{
    PyObject* _retval = _call_python_function("jouer_tour");
    try {
        Py_XDECREF(_retval);
    } catch (...) {
        PyErr_Print();
        abort();
    }
}

extern "C" void repondre_action_choix_trois()
{
    PyObject* _retval = _call_python_function("repondre_action_choix_trois");
    try {
        Py_XDECREF(_retval);
    } catch (...) {
        PyErr_Print();
        abort();
    }
}

extern "C" void repondre_action_choix_paquets()
{
    PyObject* _retval = _call_python_function("repondre_action_choix_paquets");
    try {
        Py_XDECREF(_retval);
    } catch (...) {
        PyErr_Print();
        abort();
    }
}

extern "C" void fin_jeu()
{
    PyObject* _retval = _call_python_function("fin_jeu");
    try {
        Py_XDECREF(_retval);
    } catch (...) {
        PyErr_Print();
        abort();
    }
}
