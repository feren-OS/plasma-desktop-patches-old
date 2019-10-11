/*
 *   Copyright 20016 Aleix Pol Gonzalez <aleixpol@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QApplication>
#include <KConfigGroup>
#include <KIconTheme>
#include <KSharedConfig>
#include <KSharedDataCache>
#include <KBuildSycocaProgressDialog>
#include <QDebug>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    if (app.arguments().count() != 2) {
        return 1;
    }

    {
        //KNS will give us a path
        QString themeName = app.arguments().last();
        int idx = themeName.lastIndexOf('/');
        if (idx>=0) {
            themeName = themeName.mid(idx);
        }

        KConfigGroup config(KSharedConfig::openConfig(QStringLiteral("kdeglobals"), KConfig::SimpleConfig), "Icons");
        config.writeEntry("Theme", themeName);
        config.sync();
    }

    KIconTheme::reconfigure();

    KSharedDataCache::deleteCache(QStringLiteral("icon-cache"));

    for (int i=0; i<KIconLoader::LastGroup; i++)
    {
        KIconLoader::emitChange(KIconLoader::Group(i));
    }

    KBuildSycocaProgressDialog::rebuildKSycoca(nullptr);
    return 0;
}
