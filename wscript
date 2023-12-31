#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

import glob
import os.path
import shutil

from wafgenerator import generator_player_install


def options(opt):
    pass


def configure(cfg):
    pass

def _copy_assets(source_dir, build_dir):
    assets = [
        'src/tests/test-map1.txt'
    ]
    for asset in assets:
        src_path = os.path.join(source_dir, asset)
        dst_path = os.path.join(build_dir, asset)
        try:
            shutil.copyfile(src_path, dst_path)
        except IOError:
            pass


def build(bld):
    TARGET = 'hanamikoji'
    bld.shlib(
        source='''
            src/action_action_valider.cc
            src/action_action_defausser.cc
            src/action_action_choix_trois.cc
            src/action_action_choix_paquets.cc
            src/action_repondre_choix_trois.cc
            src/action_repondre_choix_paquets.cc
            src/api.cc
            src/cardset.cc
            src/constants.cc
            src/dumper.cc
            src/entry.cc
            src/game_state.cc
            src/interface.cc
            src/rules.cc
        ''',
        defines=['MODULE_COLOR=ANSI_COL_BROWN', 'MODULE_NAME="rules"'],
        target=TARGET,
        use=['stechec2'],
    )

    if not bool(os.environ.get("WSCRIPT_SKIP_TESTS")):
      source_dir = os.path.join(bld.run_dir, 'games', TARGET)
      build_dir = os.path.join(bld.out_dir, 'games', TARGET)
      _copy_assets(source_dir, build_dir)


    abs_pattern = os.path.join(bld.path.abspath(), 'src/tests/test-*.cc')
    for test_src in glob.glob(abs_pattern):

        test_name = os.path.split(test_src)[-1]
        test_name = test_name[len("test-"):-(len(".cc"))]

        # Waf requires a relative path for the source
        src_relpath = os.path.relpath(test_src, bld.path.abspath())

        bld.program(
            features='gtest',
            source=src_relpath,
            target='hanamikoji-test-{}'.format(test_name),
            use=['hanamikoji', 'stechec2-utils'],
            includes=['.'],
            defines=['MODULE_COLOR=ANSI_COL_PURPLE',
                     'MODULE_NAME="hanamikoji"'],
        )

    bld.install_files('${PREFIX}/share/stechec2/hanamikoji', [
        'hanamikoji.yml',
    ])

    generator_player_install(bld, 'hanamikoji')
