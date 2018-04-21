# Git
## Cool Ways of Staying Up to Date
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

## Undoing Things Localy
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

## Staying up to Date
`git fetch <remote name>` will fetch changes from the given branch but will not merge
them.
If you have a branch set up to track a remote branch, you can use the `git
pull` command to automatically fetch and mergea remote branch into the current
branch.
`git clone`, for example, automatically sets up your local master branch to
track the remote master (default) branch.

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

### Sharing Tags
Same as pushing a branch
```
git push <remote name> <tag anme>
# or
git push <remote name> --tags
```

### Checking out a Tag
Create a new branch at a specific branch:
```
git checkout -b <branch> <tag>
```


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


### Merging
```
git checkout master
git merge <branch name>
git branch -d <branch name>    # Delete branch.
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

Or try out `git mergetool` - recomend you run,
```
git config --global merge.tool vimdiff
```
