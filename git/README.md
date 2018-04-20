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
