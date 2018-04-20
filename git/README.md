# Git
## Cool ways of staying up to date
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

## Undoing things localy
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

## Staying up to date
`git fetch <remote name>` will fetch changes from the given branch but will not merge
them.
If you have a branch set up to track a remote branch, you can use the `git
pull` command to automatically fetch and mergea remote branch into the current
branch.
`git clone`, for example, automatically sets up your local master branch to
track the remote master (default) branch.


