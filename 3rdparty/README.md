# 3rd party

This directory contains 3rd-party libs.

## Manual subtree strategy

After reading [this article](https://medium.com/@porteneuve/mastering-git-subtrees-943d29a798ec), I've decided to use the **manual** method of adding a subtree, not the `subtree` script available in `git`.

Here is a simple example of how `QtSuperMacros` was cloned:

```sh
git remote add QtSuperMacros http://gitlab.unique-conception.org/qt-qml-tricks/qt-supermacros.git
git fetch QtSuperMacros
git read-tree --prefix=3rdparty/QtSuperMacros -u QtSuperMacros/master
git commit -m "added subtree http://gitlab.unique-conception.org/qt-qml-tricks/qt-supermacros in 3rdparty/QtSuperMacros"
```

## List of libraries

* [QtSuperMacros](http://gitlab.unique-conception.org/qt-qml-tricks/qt-supermacros.git)
* [QtQmlModels](http://gitlab.unique-conception.org/qt-qml-tricks/qt-qml-models.git)
* [FakeIt](https://github.com/eranpeer/FakeIt.git)
