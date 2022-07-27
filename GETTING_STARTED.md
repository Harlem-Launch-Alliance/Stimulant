# Getting Started
This is a definitive guide to contributing any software to this repository. It is very likely that you will be able to skip some of these steps or do them in a different order, so don't worry if this document doesn't 100% line up with your system/environment.

This document is a work in progress. Please contact the current avionics team lead if you have any feedback.

## Development Environment
Some 3rd party software is required to make and validate changes.
### VSCode
VSCode is an IDE (integrated development environment) that provides tools to ease your development experience. [download](https://code.visualstudio.com/download)

There are many other IDEs to choose from, but if this is your first time using one VSCode is the best place to start.

Once installed, navigate to the extensions tab and install the `Arduino` extension. Some `C` or `C++` extensions may be helpful as well but aren't required.

### Arduino IDE
This IDE is required even if you plan on using a different one, since it also comes with all of the necessary libraries. [download](https://www.arduino.cc/en/software)

### Teensyduino
This add-on allows the Arduino IDE and tooling to be compatible with our Teensy microcontroller. [download](https://www.pjrc.com/teensy/td_download.html)

### Git
Not to be confused with `GitHub`, `Git` is the protocol that we use to collaborate on our software. Your computer may already come with Git. 

To check, open VSCode and open `view -> terminal`. Type `git` and hit enter. If you're not using VSCode, you can open the terminal application or powershell for this instead.

If you receive an error along the lines of `command not found`, you will need to [download/install](https://git-scm.com/downloads) git.

That's it for downloads! The rest of the steps should be relatively smooth.

## GitHub
In order to contribute, you will need to have a GitHub account. Your account does not have to be linked to an HLA email.

Once you have created an account, message the current avionics team lead to get your account added to the HLA GitHub organization. **You will not be able to easily contribute until you are added.**

## Time to code!
To make contributing as beginner friendly as possible, there is a specific workflow all members will need to go through.

For your first time contributing, you'll need to download the code onto your computer.
1. Open VSCode and then open `view -> terminal`.
1. Enter `cd ~/Documents`. This will move you to your Documents folder
1. Enter `mkdir git`. This will make a folder named `git`
1. Enter `cd git`
1. Enter `git clone https://github.com/Harlem-Launch-Alliance/Catalyst-2`

Now you are ready to write some code. This is the workflow you will follow every time you want to contribute. You will need to be a member of the HLA GitHub organization to complete these steps.

1. Choose a task from the `issues` tab on GitHub. If you're not sure what issue to choose or you want to contribute something else, feel free to contact the avionics team lead for guidance.
1. Navigate to your local repository *in terminal* `cd ~/Documents/git/Catalyst-2`
1. Make a new branch with a relevant name `git checkout -b name_of_branch main`
1. Publish your branch `git push -u origin name_of_branch`
1. Open the `Catalyst-2` folder in your IDE
1. Make your changes! (Please test your changes before submitting them for review)
1. After saving your changes. Use `git commit -m "message relevant to your changes" -a` to commit your changes to your branch
1. Repeat the previous 2 steps as needed until you are satisfied with your changes.
1. Use `git push` to push the code from your local machine onto the GitHub website. You should now be able to see your most recent changes on the site.
1. On the Github website, you can change the branch to whichever branch you would like merged with the production code. Then you can select the `Compare & pull request` button to request that your new code be adopted.
1. Please give your pull request a descriptive title and a proper description of your changes along with the testing that you did for your changes (if relevant).

That's it! Please direct any feedback you have for this document to the avionics team lead. Happy coding!