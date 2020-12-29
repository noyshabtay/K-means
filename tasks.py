from invoke import task

@task
def build(ctx):
    ctx.run("python3.8.5 setup.py build_ext --inplace")

@task(aliases = ['del'])
def delete(ctx):
    ctx.run("rm mykmeanssp.cpython-38-x86_64-linux-gnu.so")