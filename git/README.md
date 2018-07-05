# Git


<!--========================================================================-->
<!--    Looking for Things                                                  -->
<!--========================================================================-->
## Useful Commands
### Log
```
git log -n 3 --abbrev-commit
```

### Origin info
```
git remote show origin
```

### Create a Branch off a Branch in a Remote
```
git branch develop origin/develop

git checkout develop
```


<!--========================================================================-->
<!-- Working in your local repository                                       -->
<!--========================================================================-->
## Basics
### Cool Ways of Staying Up to Date
* Adding patches:
  ```
  git add -p <file>
  ```

* Cool and verbose commits:
  ```
  git commit -v
  ```
  You may want to run this command before hand:
  ```
  git config --global core.editor vi
  ```

### Undoing Things Localy
* Removing a file from the staging area:
  ```
  git rm --cached <file>
  ```

* Unstaging a staged file:
  ```
  git reset HEAD <file>
  ```

* Unmodifying a modified file:
  ```
  git checkout -- <file>
  ```

### Staying up to Date
`git fetch <remote name>` will fetch changes from the given branch but will not merge
them.
If you have a branch set up to track a remote branch, you can use the `git
pull` command to automatically fetch and mergea remote branch into the current
branch.
`git clone`, for example, automatically sets up your local master branch to
track the remote master (default) branch.

In the case of local changes then you can either commit them, stash them, or
just replace with the latest version of the file:
```bash
# commit local changes
git commit -v

# stashing
git stash
git stash pop

# replace with new version
git checkout <filename>
```




<!--========================================================================-->
<!--    Tracking repos                                                      -->
<!--========================================================================-->
## Remote
### Tracking a repo
```
git remote add upstream https://github.com/SD2E/sd2e-cli

git fetch upstream

git pull upstream master
```



<!--========================================================================-->
<!--    Working with tags                                                   -->
<!--========================================================================-->
## Tags
### Creating Tags
```
git tag -a v1.3 -m "version 1.3"
git tag
```
or
```
git tag -a v1.2 <commit hash>
```

### Getting Tags
```bash
git fetch origin --tags

# List all tags
git tag

git checkout <tag name>
```

### Sharing Tags
Same as pushing a branch
```bash
git push <remote name> <tag name>
# or
git push <remote name> --tags
```

### Checking out a Tag
Create a new branch at a specific branch:
```
git checkout -b <branch> <tag>
```





<!--========================================================================-->
<!--    Working with branches                                               -->
<!--========================================================================-->
## Branches
### Creating Branches
* To create a branch,
  ```
  git branch <branch name>
  ```

* Switch branhces,
  ```
  git checkout <branch name>
  ```

Last two commands together,
```
git checkout -b <branch name>
```

To push a local branch,
```
git push -u origin <branch name>
```

### Deleting branches
```bash
# Delete a remote branch
git push origin --delete <branch>

# Delete local branch
git branch -D <branch>
```


### List all branches
```
git branch -a
```

### Merging
```bash
git checkout master
git merge <branch name>

# Delete branch.
git branch -d <branch name>
```

Merging with conflicts is a little more interesting.
Let's say you are trying to merge branch `iss53` into `master`,
```
git checkout master
git merge iss53
```

Should return some error message requesting you to fix conflicts - the process
of merging has momentarily stopped.

If you want to see which files are unmerged at any point after a merge conflict
simple run `git status`.

Go through the unmerged files and fixed any blocks with `<<<<<<<`, `=======`, and `>>>>>>>`.
After resolving each of these sections in each conflicted file, run `git add`
on them and they'll be ready to me merged.

Or try out `git mergetool`. We recomend you run,
```
git config --global merge.tool vimdiff
```
For a cool tutorial check this [out](http://www.rosipov.com/blog/use-vimdiff-as-git-mergetool/).





<!--========================================================================-->
<!--    Working with submodules                                             -->
<!--========================================================================-->
## Submodules
### Adding a submodule
```
git submodule add https://github.com/SD2E/sd2e-cli
```

This will create a file named `.gitmodules` which will have an entry as such
```
[submodule "sd2e-cli"]
    path = sd2e-cli
    url = https://github.com/SD2E/sd2e-cli
```
Note that you can go an modify this file to track a certain branch by adding
`branch = dev`.

All you have left to do is to save the changes
```
git push origin master
```

### Cloning a Project with Submodules
You can clone the project as such
```
git clone --recurse-submodules https://github.com/SD2E/sd2e-cli
```
and all the submodules will be updated with theentire state of the project
corresponsind to the saved commit.

Alternatively, after cloning you can go into the submodule and do
```
git submodule init

git submodule update
```


### Pulling in Upstream Changes
To simply pull in new work
```
git fetch && git merge
```

or simply
```
git submodule update --remote
```


### Working on a Submodule
Pulling in work from a submodule will leave the subrepository in a detached
HEAD state (no local working branch).

To get something to work on you need to check out a branch to work on and then
go as you normally would.
You can merge changes into your local work or you can try to rebase your local
work on top of new changes.

```bash
git checkout stable

git submodule update --remote --merge

# or
git submodule update --remote --rebase
```


<!--========================================================================-->
<!--    Large file related                                                  -->
<!--========================================================================-->
# Large Files
In case you went through the trouble of trying to push a large file and then
not being able to push:
```
git filter-branch --index-filter 'git rm -r --cached --ignore-unmatch <file>' HEAD
```
