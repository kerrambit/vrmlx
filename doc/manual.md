## Manual

There are two modes for this application: **single conversion** (default) and **bulk conversion**.

### Single Conversion

You do **not** specify the mode for single conversion:

```bash
./vrmlxpy <input_file> <output_file> <config_file>
```

### Bulk Conversion

For bulk conversion, you **must** specify the mode by adding the `--bulk` option:

```bash
./vrmlxpy --bulk <input_folder> <output_folder>
```

> **Note**: The `<config_file>` must be present in the `<input_folder>` when using bulk conversion.

---

## Configuration File

The `<config_file>` is a JSON file that defines behavior for the application. All options have default values.

```json
{
  "ignoreUnknownNode": false,
  "logFileName": "vrmlproc",
  "logFileDirectory": ".",
  "synonymsFile": "./synonymsFile.json",

  "exportFormat": {
    "format": "stl",
    "options": {
      "binary": true
    }
  },

  "parallelismSettings": {
    "active": true,
    "threadsNumberLimit": <system_max_threads>
  },

  "meshSimplification": {
    "active": false,
    "percentageOfAllEdgesToSimplify": 50
  },

  "IFSSettings": {
    "checkRange": true
  }
}
```

### Configuration Options Explained

- **`ignoreUnknownNode`**: Whether to ignore unrecognized VRML nodes (`false` by default).
- **`logFileName`**: Base name of the log file (`"vrmlproc"` by default).
- **`logFileDirectory`**: Directory where logs are written (`"."` by default).
- **`synonymsFile`**: Path to a JSON file defining node name synonyms (`"./synonymsFile.json"` by default).

#### `exportFormat`
- **`format`**: Output format (`"stl"` by default). Possible values: `"stl"`, `"ply"`, `"obj"`.
- **`options.binary`**: If exporting STL, whether to use binary format (`true` by default).

#### `parallelismSettings`
- **`active`**: Enable parallel execution (`true` by default).
- **`threadsNumberLimit`**: Maximum number of threads to use (defaults to maximum threads available on your system).

#### `meshSimplification`
- **`active`**: Whether to simplify meshes (`false` by default).
- **`percentageOfAllEdgesToSimplify`**: Percent of edges to simplify when simplification is enabled (`50` by default).

#### `IFSSettings`
- **`checkRange`**: Enable range checking for `IndexedFaceSet` indices (`true` by default).

You can find real example of JSON files both for Linux and Windows here: [Linux](../vrmlxConfig.linux.json),  [Windows](../vrmlxConfig.windows.json).