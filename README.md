# Introduction
Andersen is a computer language for making epic randomly generated stories and texts.

It is named after the famous danish poet Hans Christian Andersen, who wrote
many wonderful fairytales. Just think about what he could have done, if he'd
had a computer to generate the stories automatically.

This language is an easy way to randomly generate text in a structured and
controlled way.

Many random text generators uses AI or probability to generate texts, that are
not well structured and weird, with the author having only little influence on
the output.

With Andersen the input is structured, so when you get weird output it'll be
your own fault.

This is done by creating tests with placeholders that will be replaced with
more text.  For example *"Once upon a time, there was a [HERO]..."*. The
*[HERO]* may be replaced with a prince, or a princess, or anything you've
written.

# Running and Installation

See [THIS](https://github.com/DrDimon/vim-andersen) for a vim plugin with syntax highlighting.

Running the make command should compile the project.
It requires *flex* and *bison* installed.

To compile a *.and* file, cat it to Andersen like:
```
  cat examples/minimal1.and | ./Andersen
```

The seed and result is written to standard output.

An integer seed can be given as a optional parameter:
```
  cat examples/minimal1.and | ./Andersen 1616865118
```

Run the tests with *./Test.sh*.

# Syntax
See the examples folder for well... examples.
*examples/shadowrun\_mission.and* is the most complete example.

## Objects
An Andersen file contains objects with text and placeholders. When the file is
interpreted, the placeholders are replaced with text.

A simple object:

```
<TOP>
Hello world.
</TOP>
```

This would generate the text *Hello world.*

The Andersen program will always choose an object named TOP to render. If there are multiple valid objects, it'll choose a random one.

An object can contain subobjects like:
```
<TOP>
Hello world.
  <SUBOBJECT>
Text
  <SUBOBJECT>
</TOP>
```
This would also generate the text *Hello world.* since the subobject is not used.

Indentation is not required by the compiler (but needed to stay sane).

If an object does not have any subobjects, the whole object may be written on a single line,
otherwise the newlines are required.

### Names
Any names consisting of [a-zA-Z0-9\_] are valid, except *ROOT*. Don't call objects that.

*TOP* is special, since the initial object is chosen among these. There must be one or more objects with that name.

## Placeholders
The text in an object may have placeholders marked by *[...]*. This is a path
to another object whose text will replace the placeholder when rendered. Any
time there are multiple valid objects, one is randomly chosen. This is kind of
the whole point with the random generator.
```
<TOP>
Hello [SUBOBJECT].
  <SUBOBJECT>
World
  <SUBOBJECT>
  <SUBOBJECT>
there
  <SUBOBJECT>
</TOP>
```

This would output either *"Hello World."* or *"Hello there."*.

These can be recursive, and have their own subobjects.
```
<TOP>
Hello [SUBOBJECT]. [SUBOBJECT.OPINION].
  <SUBOBJECT>
World
    <OPINION>
I like you!
    </OPINION>
  <SUBOBJECT>
  <SUBOBJECT>
there
    <OPINION>
I have an opinion!
    </OPINION>
  <SUBOBJECT>
</TOP>
```

If you want objects that are outside the object with the placeholder you can
use the *ROOT* keyword. This should be the first part of the path, and should
not be used inside the path.
```
<TOP>
Hello [ROOT.OBJECT]. [ROOT.OBJECT.OPINION].
</TOP>

<SUBOBJECT>
World
  <OPINION>
I like you!
  </OPINION>
<SUBOBJECT>
<SUBOBJECT>
there
  <OPINION>
I have an opinion!
  </OPINION>
<SUBOBJECT>
```

If an object cannot be found given a path, things go wrong and everything breaks.

Currently crashing should be considered an error message.

## Variables
In the previous example *[ROOT.OBJECT]* and *[ROOT.OBJECT.OPINION]* does not refer to the same object, so the *opinion* placeholder may be replaced by the text from the other subobject.

For example the result could be *Hello World. I have an opinion!*.

This can be solved by using a variable by prefixing the path with *name:*

```
<TOP>
Hello [VARIABLE:ROOT.OBJECT]. [VARIABLE:ROOT.OBJECT.OPINION].
</TOP>

<SUBOBJECT>
World
  <OPINION>
I like you!
  </OPINION>
<SUBOBJECT>
<SUBOBJECT>
there
  <OPINION>
I have an opinion!
  </OPINION>
<SUBOBJECT>
```
Now the program will return *Hello World. I like you!*, or *Hello there. I have
an opinion!* but never a combination. This is often what we want.

## Parameters

If you want to refer to an object from a sub object, you can pass it as a parameter:
```
<TOP>
Hello [VARIABLE:ROOT.OBJECT]. [VARIABLE:ROOT.OPINION(VARIABLE)].
</TOP>

<OBJECT>
Humans
</OBJECT>
<OBJECT>
You
</OBJECT>

<OPINION(OBJ)>
I like [OBJ:ROOT.SUBOBJECT]
</OPINION>
```

This will write *Hello Humans. I like Humans.* or *Hello You. I like You.* but never *Hello Humans. I like You.*

The object is defined whether you use is as a variable name or a parameter first.

If an object takes parameters, it must always be given in the placeholder, or
it will fail. Since the object is defined when given as a parameter, you can
pass a new variable if it is not relevant later.

Multiple parameters can be passed by separating them with a '*,*'.
```
<TOP>
Hello [ROOT.OBJECT(PARAM1, PARAM2)].
</TOP>

<OBJECT(PARAM1, PARAM2)>
World
</OBJECT>
```

## Expressions

Objects support using expressions to decide what objects are appropriate. A
series of expressions may be included inside '{}' in the text object, separated
with a ';':

```
<OBJECT(PARAM) {PARAM.varname = 1 + PARAM.varname; PARAM.varname <= 2;}>
tekst [ROOT.OBJECT(PARAM)]
</OBJECT>

<OBJECT(PARAM) {PARAM.varname > 2;}>
tekst
</OBJECT>
```

There are two kinds of expressions. Assignments and Evaluations.

### Evaluations:

When a textobeject is considered as a replacement, any expressions are
evaluated. If all expressions in the given textobject is true, then that object
is added to the pool of possible replacements, and if one or more is evaluated
to false, the object is not considered.

At this point any assignments are considered true, and not evaluated.

A variable name need not be defined or have a value assigned. If not, it will
evaluate to 0.

### Assignments:

An assignment is an expression of the form: "PARAMETER\_NAME.variable\_name =
[expression];". The names may have any casing. The expression is evaluated as
an integer and assigned.

Variables may be assigned any integer number. 0 is considered false, and
anything else true.

Assignments are only evaluated when the text object is selected. This means
that in the example above, if PARAM.varname == 2 then the increment does not
happen until it is selected, which is after the second expression
(PARAM.varname <= 2) is evaluated.

In short: expressions are evaluated twice. First all objects that are
considered for replacement without assignments, then the single object that was
selected has its assignments evaluated.

### Examples

Expressions can be used to repeat textobjects a certain number of times, as the
previous example. It will print *tekst tekst tekst tekst* (assuming varname is
0 or not set when called.

Or it can be used to make certain textobjects available under certain
conditions. For example the hero may only eat an apple if they picked it up
earlier in the story.

See examples/expressions\*.and for further examples.

## Comments

Use // to create comments. They may be a little bit funky regarding newlines. I'm working on that...
