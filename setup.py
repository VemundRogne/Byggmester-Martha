import setuptools

with open('requirements.txt') as f:
    requirements = f.read().splitlines()


setuptools.setup(
    name="byggern",
    install_requires=requirements,
    packages = setuptools.find_packages(),
    python_requires='>=3.7'
)